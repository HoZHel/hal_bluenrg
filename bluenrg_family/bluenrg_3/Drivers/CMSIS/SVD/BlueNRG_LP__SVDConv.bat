
copy  ".\BlueNRG_LP.xml"  ".\BlueNRG_LP.svd"

"SVDConv.exe" "BlueNRG_LP.svd" --generate=header --fields=macro  -o "." --debug-headerfile > log_svd_generate_header.txt

:"SVDConv.exe" "BlueNRG_LP.svd" --generate=sfr > log_svd_generate_sfr.txt

@start  .
:open output directory
:@start  C:\Workspace\BlueNRG-LP\Drivers\CMSIS\Device\ST\BlueNRG_LP\Include
:open notepad++ an load some files generated by this batch
:@start /D "C:\Program Files (x86)\" Notepad++.exe 
:@start /D "C:\Program Files (x86)\" Notepad++.exe  "C:\Workspace\BlueNRG-LP\Drivers\CMSIS\SVD\log_svd_generate_header.txt"
:@start /D "C:\Program Files (x86)\" Notepad++.exe  "C:\Workspace\BlueNRG-LP\Drivers\CMSIS\Device\ST\BlueNRG_LP\Include\BlueNRG_LP.h"
:@start /D "C:\Program Files (x86)\" Notepad++.exe  "C:\Workspace\BlueNRG-LP\Drivers\CMSIS\Device\ST\BlueNRG_LP\Include\BlueNRG_LP.h"


:pause
