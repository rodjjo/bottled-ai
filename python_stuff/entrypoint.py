try:
    # from transformers.utils import hub as tqdm_module
    # from transformers.utils.hub import tqdm as old_tqdm
    
    # from bottled_ai import progress

    # class NewTqdm:
    #     def __init__(self, *args, **kwargs):
    #         self.total = None
    #         self.tqdm = old_tqdm(*args, **kwargs)
    #         self.old_update = self.tqdm.update
    #         setattr(self.tqdm, 'update', self.update)
    #         if 'total' in kwargs:
    #             self.total = kwargs['total']

    #     def update(self, value):
    #         self.old_update(value)
    #         if self.total is not None:
    #             progress(int(value), int(self.total))

    # def new_tqdm(*args, **kwargs):
    #     return NewTqdm(args, kwargs).tqdm
    
    # setattr(tqdm_module, 'tqdm', new_tqdm)
    
    from models.listing import list_models  # no-qa
    from models.downloader import download_model, remove_model # no-qa
    from models.inference import generate_text  # no-qa
    from text_helper.text_helper import convert2html

except Exception as ex:
    with open("exception.txt", 'w') as fp:
        fp.write(str(ex))
        fp.flush()
    raise


if __name__ == '__main__':
    print('models module loaded')