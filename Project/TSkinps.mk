
TSkinps.dll: dlldata.obj TSkin_p.obj TSkin_i.obj
	link /dll /out:TSkinps.dll /def:TSkinps.def /entry:DllMain dlldata.obj TSkin_p.obj TSkin_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del TSkinps.dll
	@del TSkinps.lib
	@del TSkinps.exp
	@del dlldata.obj
	@del TSkin_p.obj
	@del TSkin_i.obj
