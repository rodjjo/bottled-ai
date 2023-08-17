import gc
import os

from transformers.utils.hub import cached_file
from models.paths import CACHE_DIR
from models.loader import unload_model
from models.listing import MODELS_MAP

from bottled_ai import progress_title

def download_model(repo_id: str) -> bool:
    progress_title(f"Downloading the model {repo_id}. Please wait")
    if os.path.exists(MODELS_MAP[repo_id]['dirname']):
        return True
    return cached_file(
        repo_id,
        MODELS_MAP[repo_id]['model_basename'] + '.safetensors',
        cache_dir=CACHE_DIR,
    ) is not None
