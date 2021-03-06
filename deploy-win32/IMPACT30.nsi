; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "IMPACT"
!define PRODUCT_VERSION "3.0 (Beta 1)"
!define PRODUCT_PUBLISHER "ILRI"
!define PRODUCT_WEB_SITE "www.ilri.org"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\CrossImpact.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "c:\IMPACT30-Setup.exe"
InstallDir "$PROGRAMFILES\IMPACT30"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "QtXmlPatterns4.dll"
  File "QtXml4.dll"
  File "QtWebKit4.dll"
  File "QtSvg4.dll"
  File "QtSql4.dll"
  File "QtNetwork4.dll"
  File "QtGui4.dll"
  File "QtCore4.dll"
  File "qsqlite4.dll"
  File "phonon4.dll"
  File "mingwm10.dll"
  File "libgcc_s_dw2-1.dll"
  File "impwidgetsplugin.dll"
  File "impmainmodules.dll"
  File "CrossImpact.exe"
  CreateDirectory "$SMPROGRAMS\IMPACT 3.0"
  CreateShortCut "$SMPROGRAMS\IMPACT 3.0\IMPACT.lnk" "$INSTDIR\CrossImpact.exe"
  CreateShortCut "$DESKTOP\IMPACT.lnk" "$INSTDIR\CrossImpact.exe"
  SetOutPath "$INSTDIR\db"
  File "db\impact.sqlite"
  SetOutPath "$INSTDIR\sqldrivers"
  File "sqldrivers\qsqlite4.dll"
  SetOutPath "$INSTDIR\tmp"
  File "tmp\eco01.html"
  SetOutPath "$INSTDIR\3rdPartyTools\highcharts\js"
  SetOverwrite try
  File "3rdPartyTools\highcharts\js\highcharts.js"
  SetOutPath "$INSTDIR\3rdPartyTools\highcharts\js\themes"
  File "3rdPartyTools\highcharts\js\themes\gray.js"
  File "3rdPartyTools\highcharts\js\themes\dark-green.js"
  File "3rdPartyTools\highcharts\js\themes\skies.js"
  File "3rdPartyTools\highcharts\js\themes\grid.js"
  File "3rdPartyTools\highcharts\js\themes\dark-blue.js"
  SetOutPath "$INSTDIR\3rdPartyTools\highcharts\js"
  File "3rdPartyTools\highcharts\js\jquery.min.js"
  File "3rdPartyTools\highcharts\js\highcharts.src.js"
  SetOutPath "$INSTDIR\3rdPartyTools\highcharts\js\adapters"
  File "3rdPartyTools\highcharts\js\adapters\mootools-adapter.src.js"
  File "3rdPartyTools\highcharts\js\adapters\prototype-adapter.js"
  File "3rdPartyTools\highcharts\js\adapters\mootools-adapter.js"
  File "3rdPartyTools\highcharts\js\adapters\prototype-adapter.src.js"
  SetOutPath "$INSTDIR\3rdPartyTools\highcharts\js\modules"
  File "3rdPartyTools\highcharts\js\modules\exporting.js"
  File "3rdPartyTools\highcharts\js\modules\canvas-tools.src.js"
  File "3rdPartyTools\highcharts\js\modules\canvas-tools.js"
  File "3rdPartyTools\highcharts\js\modules\exporting.src.js"
SectionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  CreateShortCut "$SMPROGRAMS\IMPACT 3.0\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\CrossImpact.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\CrossImpact.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\modules\exporting.src.js"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\modules\canvas-tools.js"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\modules\canvas-tools.src.js"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\modules\exporting.js"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\adapters\prototype-adapter.src.js"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\adapters\mootools-adapter.js"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\adapters\prototype-adapter.js"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\adapters\mootools-adapter.src.js"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\highcharts.src.js"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\jquery.min.js"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\themes\dark-blue.js"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\themes\grid.js"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\themes\skies.js"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\themes\dark-green.js"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\themes\gray.js"
  Delete "$INSTDIR\3rdPartyTools\highcharts\js\highcharts.js"
  Delete "$INSTDIR\tmp\eco01.html"
  Delete "$INSTDIR\sqldrivers\qsqlite4.dll"
  Delete "$INSTDIR\db\impact.sqlite"
  Delete "$INSTDIR\CrossImpact.exe"
  Delete "$INSTDIR\impmainmodules.dll"
  Delete "$INSTDIR\impwidgetsplugin.dll"
  Delete "$INSTDIR\libgcc_s_dw2-1.dll"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\phonon4.dll"
  Delete "$INSTDIR\qsqlite4.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtSql4.dll"
  Delete "$INSTDIR\QtSvg4.dll"
  Delete "$INSTDIR\QtWebKit4.dll"
  Delete "$INSTDIR\QtXml4.dll"
  Delete "$INSTDIR\QtXmlPatterns4.dll"

  Delete "$SMPROGRAMS\IMPACT 3.0\Uninstall.lnk"
  Delete "$DESKTOP\IMPACT.lnk"
  Delete "$SMPROGRAMS\IMPACT 3.0\IMPACT.lnk"

  RMDir "$SMPROGRAMS\IMPACT 3.0"
  RMDir "$INSTDIR\tmp"
  RMDir "$INSTDIR\sqldrivers"
  RMDir "$INSTDIR\db"
  RMDir "$INSTDIR\3rdPartyTools\highcharts\js\themes"
  RMDir "$INSTDIR\3rdPartyTools\highcharts\js\modules"
  RMDir "$INSTDIR\3rdPartyTools\highcharts\js\adapters"
  RMDir "$INSTDIR\3rdPartyTools\highcharts\js"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd