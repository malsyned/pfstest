import sys
if (sys.version_info[0] >= 3):
    from io import StringIO
    from unittest.mock import MagicMock
    from unittest import mock
else:
    from StringIO import StringIO
    from mock import MagicMock
    import mock

__all__ = ['StringIO', 'mock']
