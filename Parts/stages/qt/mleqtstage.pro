TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
    src \
    test

src.file = src/qtstage.pro
test.file = test/unittest.pro

#app.depends = src
test.depends = src
