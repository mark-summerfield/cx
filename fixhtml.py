#!/usr/bin/env python3
# Copyright © 2024 Mark Summerfield. All rights reserved.

import re

flags = re.MULTILINE|re.DOTALL
param_rx = re.compile(r'<h4.class=.parameters.*?</h4>.*?' +
                      r'<table.class=.list.*?</table>', flags=flags)
retval_rx = re.compile(r'<h4.class=.returnvalue.*?Return.Value.*?</p>',
                       flags=flags)

with open('cx.html', 'rt', encoding='utf-8') as file:
    text = file.read()
text = param_rx.sub('', text)
text = retval_rx.sub('', text)
with open('cx.html', 'wt', encoding='utf-8') as file:
    file.write(text)
