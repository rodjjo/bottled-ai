import traceback

import torch
import gc
import random
from datetime import datetime
import transformers

from models.loader import get_model, set_max_memory
from models.listing import MODELS_MAP
from text_helper.text_helper import convert2html


from bottled_ai import progress_text, progress_title, progress, progress_canceled

class Streamer:
    def __init__(self, tokenizer, skip_special_tokens=True):
        self.tokenizer = tokenizer
        self.firstRun = True
        self.skip_special_tokens = skip_special_tokens

    def decode(self, output_ids):
        return self.tokenizer.decode(output_ids, self.skip_special_tokens)

    def put(self, output_ids):
        if self.firstRun:
            self.firstRun = False
            return
        reply = self.decode(output_ids[0])
        # Prevent LlamaTokenizer from skipping a space
        if type(self.tokenizer) in [transformers.LlamaTokenizer, transformers.LlamaTokenizerFast] and len(output_ids) > 0:
            try:
                element = [int(xs) for xs in output_ids[0]]
            except TypeError:
                element = [int(output_ids.item())]
            if self.tokenizer.convert_ids_to_tokens(element)[0].startswith('▁'):
                reply = ' ' + reply 
        progress_text(reply)

    def end(self):
        pass


class CanceledChecker:
    def __call__(self, *args, **kwargs) -> bool:
        return progress_canceled()


def set_manual_seed(seed):
    seed = int(seed)
    if seed == -1:
        seed = random.randint(1, 2**31)

    torch.manual_seed(seed)
    if torch.cuda.is_available():
        torch.cuda.manual_seed_all(seed)

    return seed

@torch.no_grad()
def generate_text(model_id: str, params: dict) -> dict:
    try:
        prompt = params['prompt']
        context = params.get('context') or 'You are a helpful AI assistant.'
        max_new_tokens = params.get('max_new_tokens', 512)
        temperature = params.get('temperature', 1)
        top_p = params.get('top_p', 1)
        top_k = params.get('top_k', 0)
        repetition_penalty = params.get('repetition_penalty', 1)
        set_max_memory(params.get('mem_gpu', -1), params.get('mem_cpu', -1))

        progress_title("Loading the model")
        progress(0, 100)
        model, tokenizer = get_model(model_id)
        if model is None:
            return {
                "html": "no model loaded",
                "raw": "no model loaded"
            }
        progress_title("Generating text")

        set_manual_seed(-1)

        cfg = MODELS_MAP[model_id]
        fallBackTemplate = '{instruction}\n\nUSER: {input}\nASSISTANT:'
        for template in cfg['templates']:
            if '{instruction}' in template and '{input}' in template:
                hasTemplate = True
                fallBackTemplate = template
                break
            elif '{input}' in template:
                fallBackTemplate = template
        if '{instruction}' in fallBackTemplate:
            prompt = fallBackTemplate.format(instruction=context, input=prompt)
        else:
            prompt = fallBackTemplate.format(input=prompt)

        response_prefix = cfg['response_after']

        if cfg['loader'] == 'ctransformers':
            inputs = str(prompt)
        else:
            inputs = tokenizer.encode(str(prompt), return_tensors='pt', add_special_tokens=True).to('cuda:0')
        if cfg['loader'] == 'ctransformers':
            output = ''
            for txt in model(
                        prompt=inputs,
                        max_new_tokens=max_new_tokens,
                        temperature=temperature,
                        top_p=top_p,
                        top_k=int(top_k),
                        repetition_penalty=repetition_penalty,
                        stream=True
                    ):
                progress_text(txt)
                output = output + txt
                if progress_canceled():
                    break
        else:
            output = model.generate(
                inputs=inputs,
                max_new_tokens=max_new_tokens,
                temperature=temperature,
                top_p=top_p,
                top_k=top_k,
                repetition_penalty=repetition_penalty,
                streamer=Streamer(tokenizer),
                stopping_criteria=transformers.StoppingCriteriaList() + [
                    CanceledChecker()
                ]
            )[0]

        new_tokens = len(output) - len(inputs[0])
        if cfg['loader'] == 'ctransformers':
            response = output
        else:
            response = tokenizer.decode(output[-new_tokens:], True)

        if 'ASSISTANT:' in response_prefix:
            response = response.split('\n');
            for i, v in enumerate(response):
                if not v.startswith(response_prefix):
                    continue
                v = v.split(response_prefix, maxsplit=1)
                response[i] = v[0] if len(v) < 2 else v[1]
            response = '\n'.join(response)

        del inputs
        gc.collect()
        return {
            "html": convert2html(response),
            "raw": response
        }
    except Exception as ex:
        return {
            "html": convert2html(str(ex) + str(traceback.format_exc())),
            "raw": str(ex),
        }