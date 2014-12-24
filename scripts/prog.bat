set prg="C:\Program Files\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.EXE"

set binary="d:\Source\IMS\Debug\IMS.hex"

If Not Exist %binary% (
	echo File not exist
	exit 1
)

%prg% -c SWD UR -ME -P %binary% -Rst -Run
