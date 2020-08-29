from typing import Any


class CppClass:
    _obj: Any

    @property
    def obj(self):
        return self._obj
