
import os
from typing import List

from models.paths import CACHE_DIR

MODEL_LIST = [{
        'id': 'TheBloke/Nous-Hermes-13B-GPTQ',
        'name': 'Nous-Hermes-13b (GPTQ)',
        'model_basename': 'nous-hermes-13b-GPTQ-4bit-128g.no-act.order',
        'dirname': os.path.join(CACHE_DIR, 'models--TheBloke--Nous-Hermes-13B-GPTQ'),
        'loader': 'auto_gpt',
        'locally': False,
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
            '### Instruction:\n<instruction>\n### Input:\n<input>\n### Response:\n',
            '### Instruction:\n<instruction>\n### Response:\n',
        ]
    }, {
        'id': 'TheBloke/Wizard-Vicuna-13B-Uncensored-GPTQ',
        'name': 'Wizard-Vicuna-13B-Uncensored-GPTQ',
        'model_basename': 'Wizard-Vicuna-13B-Uncensored-GPTQ-4bit-128g.compat.no-act-order',
        'dirname': os.path.join(CACHE_DIR, 'models--TheBloke--Wizard-Vicuna-13B-Uncensored-GPTQ'),
        'loader': 'auto_gpt',
        'locally': False, 
        'description': (
            'This is wizard-vicuna-13b trained with a subset of the dataset - responses that contained alignment / moralizing were removed.\n'
            'The intent is to train a WizardLM that doesn\'t have alignment built-in, so that alignment (of any sort) can be added separately '
            'with for example with a RLHF LoRA.\n'
            'An uncensored model has no guardrails.\n'
            'You are responsible for anything you do with the model, just as you are responsible for anything you do with any dangerous object such as a knife, gun, lighter, or car.'
        ),
        'response_after':'### Response:\n',
        'templates': [
            '### Instruction:\n<instruction>\n### Input:\n<input>\n### Response:\n',
            '### Instruction:\n<instruction>\n### Response:\n',
        ]
    }
]

MODELS_MAP = {
    v['id']: v for v in MODEL_LIST
}

def list_models() -> List[dict]:
    for element in MODEL_LIST:
        element['locally'] = os.path.exists(
            element['dirname']
        )
    return MODEL_LIST