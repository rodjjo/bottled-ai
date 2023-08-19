from typing import Tuple, List
import os
import gc
import torch
from transformers import AutoTokenizer
from auto_gptq import AutoGPTQForCausalLM
from models.paths import CACHE_DIR
from models.listing import MODELS_MAP

SELECTED_MODEL = None
MAX_MEMORY = None

def set_max_memory(gpu, cpu):
    global MAX_MEMORY
    MAX_MEMORY = None
    if gpu > 100 and cpu > 100:
        MAX_MEMORY = {
            0: f"{gpu/ 1024.0}GiB",
            'cpu': f"{cpu / 1024.0}GiB"
        }

def get_models_file(repo_id) -> List[str]:
    mdl = MODELS_MAP[repo_id]
    basedir = os.path.join(mdl['dirname'], 'snapshots') 
    if not os.path.exists(basedir):
        contents = []
    else:
        contents = os.listdir(basedir)
    for d in contents:
        if len(d) != 40:
            continue
        p = os.path.join(basedir, d)
        if os.path.isdir(p):
            basedir = p
            break
    params = [
        os.path.join(basedir, 'tokenizer.model'),
    ]
    return [
        os.path.join(basedir, f'{mdl["model_basename"]}.safetensors'),
        os.path.join(basedir, 'config.json'),
        os.path.join(basedir, 'quantize_config.json'),
        os.path.join(basedir, 'tokenizer.json'),
        os.path.join(basedir, 'tokenizer_config.json'),
        os.path.join(basedir, 'special_tokens_map.json'),
        # os.path.join(basedir, 'added_tokens.json'),
    ] + params

def have_local_model(repo_id) -> bool:
    files = get_models_file(repo_id)
    if len(files) < 1:
        return False
    for f in files:
        if not os.path.exists(f):
            return False
    return True

def get_model(repo_id: str) -> Tuple[AutoGPTQForCausalLM, AutoTokenizer]:
    select_model(repo_id)
    if SELECTED_MODEL is None:
        return None, None
    return SELECTED_MODEL['model'], SELECTED_MODEL['tokenizer']


def select_model(repo_id: str) -> bool:
    global SELECTED_MODEL
    if SELECTED_MODEL is not None and SELECTED_MODEL['repo_id'] == repo_id:
        return True
    unload_model()
    if not have_local_model(repo_id):
        return False
    params = dict(
        device="cuda:0", 
        use_safetensors=True, 
        use_triton=False,
        cache_dir=CACHE_DIR,
        model_basename=MODELS_MAP[repo_id]['model_basename'],
        local_files_only=True,
        trust_remote_code=True
    )
    if MAX_MEMORY:
        params['max_memory'] = MAX_MEMORY
    model = AutoGPTQForCausalLM.from_quantized(
        repo_id, 
        **params
    )

    if hasattr(model, 'model'):
        if not hasattr(model, 'dtype'):
            if hasattr(model.model, 'dtype'):
                model.dtype = model.model.dtype

        if hasattr(model.model, 'model') and hasattr(model.model.model, 'embed_tokens'):
            if not hasattr(model, 'embed_tokens'):
                model.embed_tokens = model.model.model.embed_tokens

            if not hasattr(model.model, 'embed_tokens'):
                model.model.embed_tokens = model.model.model.embed_tokens

    SELECTED_MODEL = {
        'model': model,
        'tokenizer': AutoTokenizer.from_pretrained(
            repo_id, 
            **params
        ),
        'repo_id': repo_id
    }


def unload_model():
    global SELECTED_MODEL
    SELECTED_MODEL = None
    gc.collect()
    torch.cuda.empty_cache()