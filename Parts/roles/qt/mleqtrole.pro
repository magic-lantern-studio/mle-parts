TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
    src \
    test

src.file = src/qtrole.pro
test.file = test/unittest.pro

#app.depends = src
test.depends = src
