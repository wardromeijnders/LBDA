from ctypes import CDLL
from functools import lru_cache


@lru_cache(1)
def get_library():
    # TODO different for Unix/Windows
    return CDLL("lib/libsmips.so")
