TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
    src \
    test

src.file = src/qtset.pro
test.file = test/unittest.pro

#app.depends = src
test.depends = src
