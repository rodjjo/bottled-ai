from models.loader import get_model
from transformers import TextGenerationPipeline

from bottled_ai import progress_title, progress


def generate_text(model_id: str, input: str) -> dict:
    global pipeline
    progress_title("Generating text. Please wait")
    progress(0, 100)
    model, tokenizer = get_model(model_id)
    if model is None:
        return {
            "response": "no model loaded",
        }
    pipeline = TextGenerationPipeline(model=model, tokenizer=tokenizer, device='cuda:0')
    response = pipeline(input, max_new_tokens=2048)[0]["generated_text"]
    response = response.split("### Response:\n", maxsplit=1)
    if len(response) == 2:
        response = response[1]
    else:
        response = response[0]
    response = response.replace('. ', '.\n')
    response = response.replace('Mr.\n', 'Mr. ')
    response = response.replace('Mrs.\n', 'Mrs. ')
    return {
        "response": response
    }