#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#connect to our cluster
from elasticsearch import Elasticsearch
import json
from os import listdir
from os.path import isfile, join

mypath = "./data/"
onlyfiles = [f for f in listdir(mypath) if isfile(join(mypath, f))]




es = Elasticsearch([{'host': 'localhost', 'port': 9200}])
for file in onlyfiles:
  if ".txt" in file: # we only want txt file not tag file
    print (file)
    with open(mypath+file, encoding='utf-8', errors='ignore') as f:
      content = f.readlines()
    data_id, data_title, data_content = content;
    es.index(index='posts', doc_type='cjk_book', body={
    'id': data_id,
    'title': data_title,
    'content': data_content,
    }, request_timeout = 50) # the larger the file, the bigger the request_timeout should be
