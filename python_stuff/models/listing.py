
import os
from typing import List

from models.paths import CACHE_DIR

_MODEL_LIST = [{
        'id': 'TheBloke/Nous-Hermes-13B-GPTQ',
        'name': 'Nous-Hermes-13b (GPTQ)',
        'model_basename': 'nous-hermes-13b-GPTQ-4bit-128g.no-act.order',
        'dirname': os.path.join(CACHE_DIR, 'models--TheBloke--Nous-Hermes-13B-GPTQ'),
        'loader': 'auto_gptq',
        'locally': False,
        'can_remove': True,
        'description': (
            'Nous-Hermes-13b is a state-of-the-art language model fine-tuned on over 300,000 instructions.'
            'This model was fine-tuned by Nous Research, with Teknium and Karan4D leading the fine tuning process and dataset curation,'
            ' Redmond AI sponsoring the compute, and several other contributors. The result is an enhanced Llama 13b model '
            'that rivals GPT-3.5-turbo in performance across a variety of tasks.'
            'This model stands out for its long responses, low hallucination rate, and absence of OpenAI censorship mechanisms.'
            'The fine-tuning process was performed with a 2000 sequence length on an 8x a100 80GB DGX machine for over 50 hours.'
        ),
        'response_after':'### Response:\n',
        'templates': [
            '### Instruction:\n{instruction}\n### Input:\n{input}\n### Response:\n',
        ]
    }, {
        'id': 'TheBloke/Wizard-Vicuna-13B-Uncensored-GPTQ',
        'name': 'Wizard-Vicuna-13B-Uncensored-GPTQ',
        'model_basename': 'Wizard-Vicuna-13B-Uncensored-GPTQ-4bit-128g.compat.no-act-order',
        'dirname': os.path.join(CACHE_DIR, 'models--TheBloke--Wizard-Vicuna-13B-Uncensored-GPTQ'),
        'loader': 'auto_gptq',
        'locally': False, 
        'can_remove': True,
        'description': (
            'This is wizard-vicuna-13b trained with a subset of the dataset - responses that contained alignment / moralizing were removed.\n'
            'The intent is to train a WizardLM that doesn\'t have alignment built-in, so that alignment (of any sort) can be added separately '
            'with for example with a RLHF LoRA.\n'
            'An uncensored model has no guardrails.\n'
            'You are responsible for anything you do with the model, just as you are responsible for anything you do with any dangerous object such as a knife, gun, lighter, or car.'
        ),
        'response_after':'### Response:\n',
        'templates': [
            '### Instruction:\n{instruction}>\n### Input:\n{input}>\n### Response:\n',
        ]
    }, {
        'id': 'TheBloke/openchat_v2_w-GPTQ',
        'name': 'openchat_v2_w-GPT',
        'model_basename': 'openchat_v2_w-GPTQ-4bit-128g.no-act.order',
        'dirname': os.path.join(CACHE_DIR, 'models--TheBloke--openchat_v2_w-GPT'),
        'loader': 'auto_gptq_openchat',
        'locally': False, 
        'can_remove': True,
        'description': (
            'The OpenChat v2 family is inspired by offline reinforcement learning, '
            'including conditional behavior cloning (OpenChat-v2) and weighted behavior cloning (OpenChat-v2-w).'
        ),
        'response_after':'### Response:\n',
        'templates': []
    },
    {
        'id': 'TheBloke/WizardLM-7B-uncensored-GPTQ',
        'name': 'WizardLM-7B-uncensored-GPTQ',
        'model_basename': 'WizardLM-7B-uncensored-GPTQ-4bit-128g.compat.no-act-order',
        'dirname': os.path.join(CACHE_DIR, 'models--TheBloke--WizardLM-7B-uncensored-GPTQ'),
        'loader': 'auto_gptq',
        'locally': False, 
        'can_remove': True,
        'description': (
           'This is WizardLM trained with a subset of the dataset - responses that contained alignment / moralizing were removed.\n' 
           'The intent is to train a WizardLM that doesn\'t have alignment built-in, so that alignment (of any sort) can be added separately '
           'with for example with a RLHF LoRA.'
        ),
        'response_after': '### Response:\n',
        'templates': [
            '### Instruction:\n{instruction}>\n### Input:\n{input}>\n### Response:\n',
        ]
    },
    {
        'id': 'TheBloke/Nous-Hermes-Llama-2-7B-GPTQ',
        'name': 'Nous-Hermes-Llama-2-7B-GPTQ',
        'model_basename': 'gptq_model-4bit-128g',
        'dirname': os.path.join(CACHE_DIR, 'models--TheBloke--Nous-Hermes-Llama-2-7B-GPTQ'),
        'loader': 'auto_gptq',
        'locally': False, 
        'can_remove': True,
        'description': (
           'Nous-Hermes-Llama2-7b is a state-of-the-art language model fine-tuned on over 300,000 instructions.\n'
           'This model was fine-tuned by Nous Research, with Teknium leading the fine tuning process and dataset curation, '
           'Redmond AI sponsoring the compute, and several other contributors.'
        ),
        'response_after': '### Response:\n',
        'templates': [
            '### Instruction:\n{instruction}\n### Input:\n{input}\n### Response:\n',
        ]
    },
    {
        'id': 'TheBloke/StableBeluga-7B-GPTQ',
        'name': 'StableBeluga-7B-GPTQ',
        'model_basename': 'gptq_model-4bit-128g',
        'dirname': os.path.join(CACHE_DIR, 'models--TheBloke--StableBeluga-7B-GPTQ'),
        'loader': 'auto_gptq',
        'locally': False, 
        'can_remove': True,
        'description': (
           'Stable Beluga 7B is a Llama2 7B model finetuned on an Orca style Dataset'
        ),
        'response_after': '### Response:\n',
        'templates': [
            '### System:\n{instruction}\n### User:\n{input}\n### Response:\n',
        ]
    },
    {
        'id': 'TheBloke/stablecode-instruct-alpha-3b-GPTQ',
        'name': 'stablecode-instruct-alpha-3b-GPTQ',
        'model_basename': 'gptq_model-4bit-128g',
        'dirname': os.path.join(CACHE_DIR, 'models--TheBloke--stablecode-instruct-alpha-3b-GPTQ'),
        'loader': 'auto_gptq',
        'locally': False, 
        'can_remove': True,
        'have_tokenizer_model': False,
        'description': (
           'The model is intended to follow instruction to generate code. The dataset used to train the model is formatted in Alpaca format.'
        ),
        'response_after': '### Response:\n',
        'templates': [
            '### Instruction:\n{input}\n### Response:\n'
        ]
    }, {
        'id': 'TheBloke/orca_mini_3B-GGML',
        'name': 'orca_mini_3B-GGML',
        'model_basename': 'orca-mini-3b.ggmlv3.q8_0',
        'dirname': os.path.join(CACHE_DIR, 'models--TheBloke--orca_mini_3B-GGML'),
        'loader': 'ctransformers',
        'model_type': 'llama',
        'locally': False, 
        'can_remove': True,
        'have_tokenizer_model': False,
        'description': (
           'It runs using the CPU.\nAn OpenLLaMa-3B model model trained on explain tuned datasets, created using Instructions and Input from WizardLM, Alpaca & Dolly-V2 datasets and applying Orca Research Paper dataset construction approaches.'
        ),
        'response_after': '### Response:\n',
        'templates': [
            '### System:\n{instruction}\n### User:\n{input}\n### Response:\n',
        ]
    }, {
        'id': 'NousResearch/Nous-Hermes-Llama2-13b-GGML',
        'name': 'Nous-Hermes-Llama2-13b-GGML',
        'model_basename': 'ggml-Hermes-2-step2559-q4_K_M',
        'dirname': os.path.join(CACHE_DIR, 'models--NousResearch--Nous-Hermes-Llama2-13b-GGML'),
        'loader': 'ctransformers',
        'model_type': 'llama',
        'locally': False, 
        'can_remove': True,
        'have_tokenizer_model': False,
        'description': (
           'It runs using the CPU.\nNous-Hermes-Llama2-13b is a state-of-the-art language model fine-tuned on over 300,000 instructions. This model was fine-tuned by Nous Research, with Teknium and Emozilla leading the fine tuning process and dataset curation, Redmond AI sponsoring the compute, and several other contributors.'
        ),
        'response_after': '### Response:\n',
        'templates': [
            '### Instruction:\n{instruction}\n### Input:\n{input}\n### Response:\n',
        ]
    }
]

_MODELS_MAP = {
    v['id']: v for v in _MODEL_LIST
}

def get_additional_models(dirpath) -> List[dict]:
    result = []
    if not dirpath:
        return result
    files = os.listdir(dirpath)
    for d in sorted(files):
        if d.endswith('.bin') and 'ggml' in d:
            result.append({
                'id': os.path.join(dirpath, d),
                'name': d,
                'model_basename': d.rsplit('.', maxsplit=1)[0],
                'dirname': dirpath,
                'loader': 'ctransformers',
                'model_type': 'llama',
                'locally': False, 
                'have_tokenizer_model': False,
                'description': (
                    'A custom ggml model placed by the user at the additional models directory.\nIf you want to remove go to the directory: '
                    f'{dirpath} and remove the file {d}'
                ),
                'response_after': '### Response:\n',
                'templates': [
                    '### Instruction:\n{instruction}\n### Input:\n{input}\n### Response:\n',
                ]
            })
    return result

def get_models_map(dirname: str = ''):
    result = {
        v['id']: v for v in get_additional_models(dirname) + get_additional_models(CACHE_DIR)
    }
    result.update(_MODELS_MAP)
    if dirname != '' and CACHE_DIR not in dirname:
        for k, v in result.items():
            if CACHE_DIR not in v['dirname'] or os.path.exists(v['dirname']):
                continue
            n = v['dirname']
            n = n.replace(CACHE_DIR, '')
            if n.startswith('/') or n.startswith('\\'):
                n = n[1:]
            n = os.path.join(dirname, n)
            if os.path.exists(n):
                v['dirname'] = n
                v['cache_dir'] = dirname
            result[k] = v
    return result


def list_models(additional_dir) -> List[dict]:
    try:
        models = list(get_models_map(additional_dir).values())
        for element in models:
            element['locally'] = have_local_model(
                element['id'],
                additional_dir
            )
        return models
    except Exception as ex:
        print(str(ex))
        return []


def get_models_file(repo_id, additional_model_dir) -> List[str]:
    mdl = get_models_map(additional_model_dir)[repo_id]
    basedir = os.path.join(mdl['dirname'], 'snapshots') 
    if not os.path.exists(basedir):
        basedir = mdl['dirname']
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
    if mdl['loader'] == 'ctransformers':
        return [
            os.path.join(basedir, f'{mdl["model_basename"]}.bin'),
        ]
    if mdl.get('have_tokenizer_model', True):
        params = [
            os.path.join(basedir, 'tokenizer.model'),
        ]
    else:
        params = [
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


def have_local_model(repo_id, additional_model_dir) -> bool:
    files = get_models_file(repo_id, additional_model_dir)
    if len(files) < 1:
        return False
    for f in files:
        if not os.path.exists(f):
            return False
    return True