import logging
logging.basicConfig(filename='example.log', encoding='utf-8', level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s %(message)s')
logging.debug('This message should go to the log file')
logging.info('So should this')
logging.warning('And this, too')
logging.error('And non-ASCII stuff, too, like Øresund and Malmö')
