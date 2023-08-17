from typing import Tuple
import os
import gc
from transformers import AutoTokenizer
from auto_gptq import AutoGPTQForCausalLM
from models.paths import CACHE_DIR
from models.listing import MODELS_MAP

SELECTED_MODEL = None


def get_model(repo_id: str) -> Tuple[AutoGPTQForCausalLM, AutoTokenizer]:
    select_model(repo_id)
    return SELECTED_MODEL['model'], SELECTED_MODEL['tokenizer']


def select_model(repo_id: str) -> bool:
    global SELECTED_MODEL
    if SELECTED_MODEL is not None and SELECTED_MODEL['repo_id'] == repo_id:
        return True
    unload_model()
    params = dict(
        device="cuda:0", 
        use_safetensors=True, 
        use_triton=False,
        cache_dir=CACHE_DIR,
        model_basename=MODELS_MAP[repo_id]['model_basename'],
        local_files_only=True
    )
    SELECTED_MODEL = {
        'model': AutoGPTQForCausalLM.from_quantized(
            repo_id, 
            **params
        ),
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
