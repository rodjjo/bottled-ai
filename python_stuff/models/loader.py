from typing import Tuple, List
import os
import gc
import torch
from transformers import AutoTokenizer
from auto_gptq import AutoGPTQForCausalLM
from ctransformers import AutoModelForCausalLM

from models.paths import CACHE_DIR
from models.listing import get_models_map, get_models_file, have_local_model

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

def get_model(repo_id: str, additional_model_dir: str) -> Tuple[AutoGPTQForCausalLM, AutoTokenizer]:
    select_model(repo_id, additional_model_dir)
    if SELECTED_MODEL is None:
        return None, None
    return SELECTED_MODEL['model'], SELECTED_MODEL['tokenizer']


def select_model(repo_id: str, additional_model_dir : str) -> bool:
    global SELECTED_MODEL
    if SELECTED_MODEL is not None and SELECTED_MODEL['repo_id'] == repo_id:
        return True
    unload_model()
    if not have_local_model(repo_id, additional_model_dir):
        return False
    mdls = get_models_map(additional_model_dir)
    if mdls[repo_id]['loader'] == 'auto_gptq':
        return select_autogptq_model(repo_id, additional_model_dir)
    if mdls[repo_id]['loader'] == 'ctransformers':
        return select_ggml_model(repo_id, additional_model_dir)
    return False


def select_autogptq_model(repo_id: str, additional_model_dir: str) -> bool:
    global SELECTED_MODEL
    mdls = get_models_map(additional_model_dir)

    params = dict(
        device="cuda:0", 
        use_safetensors=True, 
        use_triton=False,
        cache_dir=mdls[repo_id].get('cache_dir', CACHE_DIR),
        model_basename=mdls[repo_id]['model_basename'],
        local_files_only=True
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


def select_ggml_model(repo_id, additional_model_dir: str):
    global SELECTED_MODEL
    mdls = get_models_map(additional_model_dir)
    params = dict(
        local_files_only=True,
        model_type=mdls[repo_id].get('model_type', 'gpt-2')
    )
    if MAX_MEMORY:
        params['max_memory'] = MAX_MEMORY
    local_file = get_models_file(repo_id, additional_model_dir)[0]
    model = AutoModelForCausalLM.from_pretrained(
        local_file, 
        **params
    )
    SELECTED_MODEL = {
        'model': model,
        'tokenizer': None,
        'repo_id': repo_id
    }



def unload_model():
    global SELECTED_MODEL
    SELECTED_MODEL = None
    gc.collect()
    torch.cuda.empty_cache()