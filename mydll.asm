format PE GUI 4.0 DLL
entry EntryPoint
 
include 'D:\fasm\INCLUDE\win32ax.inc'
include 'D:\fasm\INCLUDE\ENCODING\utf8.inc'

section '.data' readable writeable
	_MSG1 du 'ПОБЕДА', 0
	_MSG2 du 'ПЛОХО', 0
	_CPSH3 du 'РЕЗУЛЬТАТ', 0
	_KEY du 'lol', 0
	Resoult dd ?

section '.code' code readable executable
 
proc EntryPoint hinstDLL, fdwReason, lpvReserved
        mov eax, TRUE
        ret
endp

proc helloworld, msg
   invoke lstrcmpW, [msg], _KEY
   cmp [Resoult], eax
   je true
   jmp false
ret
endp

true:
   invoke MessageBoxW, 0, _MSG1, _CPSH3, 0
   invoke ExitProcess, 0

false:
   invoke MessageBoxW, 0, _MSG2, _CPSH3, 0
   invoke ExitProcess, 0

section '.idata' import data readable writeable

  library user, 'USER32.DLL', \
  kernel32, 'kernel32.dll'

  import user, \
  MessageBoxW, 'MessageBoxW'

  import kernel32, \
  ExitProcess, 'ExitProcess', \
  lstrcmpW, 'lstrcmpW'

section '.edata' export data readable
  export 'msg.DLL', helloworld, 'helloworld'
section '.reloc' fixups data discardable