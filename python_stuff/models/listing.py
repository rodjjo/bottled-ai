
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
            '### Instruction:\n{instruction}\n### Input:\n{input}\n### Response:\n',
            '### Instruction:\n{input}\n### Response:\n',
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
            '### Instruction:\n{instruction}>\n### Input:\n{input}>\n### Response:\n',
            '### Instruction:\n{input}>\n### Response:\n',
        ]
    }, {
        'id': 'TheBloke/openchat_v2_w-GPTQ',
        'name': 'openchat_v2_w-GPT',
        'model_basename': 'openchat_v2_w-GPTQ-4bit-128g.no-act.order',
        'dirname': os.path.join(CACHE_DIR, 'models--TheBloke--openchat_v2_w-GPT'),
        'loader': 'auto_gpt_openchat',
        'locally': False, 
        'description': (
            'The OpenChat v2 family is inspired by offline reinforcement learning, '
            'including conditional behavior cloning (OpenChat-v2) and weighted behavior cloning (OpenChat-v2-w).'
        ),
        'response_after':'### Response:\n',
        'templates': []
    },
    # {
    #     'id': 'TheBloke/EverythingLM-13B-16K-GPTQ',
    #     'name': 'EverythingLM-13B-16K-GPTQ',
    #     'model_basename': 'gptq_model-4bit-128g',
    #     'dirname': os.path.join(CACHE_DIR, 'models--TheBloke--EverythingLM-13B-16K-GPTQ'),
    #     'loader': 'auto_gpt',
    #     'locally': False, 
    #     'description': (
    #        ''
    #     ),
    #     'response_after': 'ASSISTANT: ',
    #     'templates': [
    #         '{instruction}>\n\nUSER: {input}\nASSISTANT:'
    #     ]
    # }, 
    {
        'id': 'TheBloke/WizardLM-7B-uncensored-GPTQ',
        'name': 'WizardLM-7B-uncensored-GPTQ',
        'model_basename': 'WizardLM-7B-uncensored-GPTQ-4bit-128g.compat.no-act-order',
        'dirname': os.path.join(CACHE_DIR, 'models--TheBloke--WizardLM-7B-uncensored-GPTQ'),
        'loader': 'auto_gpt',
        'locally': False, 
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
        'loader': 'auto_gpt',
        'locally': False, 
        'description': (
           'Nous-Hermes-Llama2-7b is a state-of-the-art language model fine-tuned on over 300,000 instructions.\n'
           'This model was fine-tuned by Nous Research, with Teknium leading the fine tuning process and dataset curation, '
           'Redmond AI sponsoring the compute, and several other contributors.'
        ),
        'response_after': '### Response:\n',
        'templates': [
            '### Instruction:\n{instruction}\n### Input:\n{input}\n### Response:\n',
            '### Instruction:\n{input}\n### Response:\n',
        ]
    },
    # {
    #     'id': 'TheBloke/WizardLM-Uncensored-Falcon-7B-GPTQ',
    #     'name': 'WizardLM-Uncensored-Falcon-7B-GPTQ',
    #     'model_basename': 'gptq_model-4bit-64g',
    #     'dirname': os.path.join(CACHE_DIR, 'models--TheBloke--WizardLM-Uncensored-Falcon-7B-GPTQ'),
    #     'loader': 'auto_gpt',
    #     'locally': False, 
    #     'tokenizerModel': False,
    #     'description': (
    #        ''
    #     ),
    #     'response_after': '### Response:\n',
    #     'templates': [
    #         '{input}\n### Response:\n',
    #     ]
    # },
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