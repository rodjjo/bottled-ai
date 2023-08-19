import gc
import os
from shutil import rmtree
from transformers.utils.hub import cached_file
from models.paths import CACHE_DIR
from models.loader import unload_model, have_local_model, get_models_file
from models.listing import MODELS_MAP

from bottled_ai import progress_title

def download_model(repo_id: str) -> bool:
    progress_title(f"Downloading the model {repo_id}. Please wait")
    if have_local_model(repo_id):
        return True
    files = get_models_file(repo_id)
    for f in files:
        if cached_file(
            repo_id,
            os.path.basename(f),
            cache_dir=CACHE_DIR,
        ) is None:
            return False
    return True

def remove_model(repo_id: str) -> bool:
    rmtree(MODELS_MAP['dirname'], ignore_errors=True)
    return False