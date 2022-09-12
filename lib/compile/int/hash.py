# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_hash')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_hash')
    _hash = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_hash', [dirname(__file__)])
        except ImportError:
            import _hash
            return _hash
        try:
            _mod = imp.load_module('_hash', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _hash = swig_import_helper()
    del swig_import_helper
else:
    import _hash
del _swig_python_version_info

try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

def _swig_setattr_nondynamic(self, class_type, name, value, static=1):
    if (name == "thisown"):
        return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name, None)
    if method:
        return method(self, value)
    if (not static):
        if _newclass:
            object.__setattr__(self, name, value)
        else:
            self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)


def _swig_setattr(self, class_type, name, value):
    return _swig_setattr_nondynamic(self, class_type, name, value, 0)


def _swig_getattr(self, class_type, name):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    raise AttributeError("'%s' object has no attribute '%s'" % (class_type.__name__, name))


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except __builtin__.Exception:
    class _object:
        pass
    _newclass = 0

class buffer(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, buffer, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, buffer, name)
    __repr__ = _swig_repr

    def __init__(self, nelements):
        this = _hash.new_buffer(nelements)
        try:
            self.this.append(this)
        except __builtin__.Exception:
            self.this = this
    __swig_destroy__ = _hash.delete_buffer
    __del__ = lambda self: None

    def __getitem__(self, index):
        return _hash.buffer___getitem__(self, index)

    def __setitem__(self, index, value):
        return _hash.buffer___setitem__(self, index, value)

    def cast(self):
        return _hash.buffer_cast(self)
    if _newclass:
        frompointer = staticmethod(_hash.buffer_frompointer)
    else:
        frompointer = _hash.buffer_frompointer
buffer_swigregister = _hash.buffer_swigregister
buffer_swigregister(buffer)

def buffer_frompointer(t):
    return _hash.buffer_frompointer(t)
buffer_frompointer = _hash.buffer_frompointer

class Hash(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Hash, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Hash, name)

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr

    def __call__(self, *args):
        return _hash.Hash___call__(self, *args)

    def add(self, *args):
        return _hash.Hash_add(self, *args)

    def get_hash(self):
        return _hash.Hash_get_hash(self)

    def reset(self):
        return _hash.Hash_reset(self)
    __swig_destroy__ = _hash.delete_Hash
    __del__ = lambda self: None
Hash_swigregister = _hash.Hash_swigregister
Hash_swigregister(Hash)

class SHA256(Hash):
    __swig_setmethods__ = {}
    for _s in [Hash]:
        __swig_setmethods__.update(getattr(_s, '__swig_setmethods__', {}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, SHA256, name, value)
    __swig_getmethods__ = {}
    for _s in [Hash]:
        __swig_getmethods__.update(getattr(_s, '__swig_getmethods__', {}))
    __getattr__ = lambda self, name: _swig_getattr(self, SHA256, name)
    __repr__ = _swig_repr
    BlockSize = _hash.SHA256_BlockSize
    HashBytes = _hash.SHA256_HashBytes

    def __init__(self):
        this = _hash.new_SHA256()
        try:
            self.this.append(this)
        except __builtin__.Exception:
            self.this = this

    def __call__(self, *args):
        return _hash.SHA256___call__(self, *args)

    def add(self, *args):
        return _hash.SHA256_add(self, *args)

    def get_hash(self):
        return _hash.SHA256_get_hash(self)

    def get_raw_hash(self):
        return _hash.SHA256_get_raw_hash(self)

    def reset(self):
        return _hash.SHA256_reset(self)
    __swig_destroy__ = _hash.delete_SHA256
    __del__ = lambda self: None
SHA256_swigregister = _hash.SHA256_swigregister
SHA256_swigregister(SHA256)

# This file is compatible with both classic and new-style classes.


