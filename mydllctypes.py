from ctypes import *
mydll = cdll.LoadLibrary('mydll.dll')
result1= mydll.helloworld('lol')