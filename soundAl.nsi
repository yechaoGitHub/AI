; 该脚本使用 HM VNISEdit 脚本编辑器向导产生

; 安装程序初始定义常量
!define PRODUCT_NAME "AiSound"
!define PRODUCT_VERSION "1.0.1"
!define PRODUCT_PUBLISHER "AiSound"
!define PRODUCT_WEB_SITE "http://www.aisound.com"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\AiSound.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma

; ------ MUI 现代界面定义 (1.67 版本以上兼容) ------
!include "MUI.nsh"

; MUI 预定义常量
!define MUI_ABORTWARNING
!define MUI_ICON "icon.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"


; 欢迎页面
!insertmacro MUI_PAGE_WELCOME
; 许可协议页面
; 安装过程页面
!insertmacro MUI_PAGE_INSTFILES
; 安装完成页面
!define MUI_FINISHPAGE_RUN "$INSTDIR\AiSound.exe"
!insertmacro MUI_PAGE_FINISH

; 安装卸载过程页面
!insertmacro MUI_UNPAGE_INSTFILES


; 安装界面包含的语言设置
!insertmacro MUI_LANGUAGE "English"


; 安装预释放文件
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI 现代界面定义结束 ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "AiSoundSetup-v${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES\AiSound"
InstallDirRegKey HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "icon.ico"
  CreateDirectory "$SMPROGRAMS\AiSound"

	File /r "Bin\*.*"
	CreateShortCut "$DESKTOP\AiSound.lnk" "$INSTDIR\AiSound.exe"
	
	SetShellVarContext all
        CreateShortCut "$SMSTARTUP\AiSound.lnk" "$INSTDIR\AiSound.exe"
SectionEnd

Section -AdditionalIcons
  CreateDirectory "$SMPROGRAMS\AiSound"
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\AiSound\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\AiSound\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\AiSound.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\AiSound.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

/******************************
 *  以下是安装程序的卸载部分  *
 ******************************/

Section Uninstall
  Delete "$DESKTOP\AiSound.lnk"
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$SMPROGRAMS\AiSound\Uninstall.lnk"
  Delete "$SMPROGRAMS\AiSound\Website.lnk"
  Delete "$DESKTOP\AiSound.lnk"
  Delete "$INSTDIR\*.*"

  RMDir "$SMPROGRAMS\AiSound"
  RMDir /r "$INSTDIR"

  RMDir /r "$INSTDIR\Log\"
  RMDir /r "$INSTDIR\iconengines\"
  RMDir /r "$INSTDIR\imageformats\"
  RMDir /r "$INSTDIR\platforms\"


  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd

#-- 根据 NSIS 脚本编辑规则，所有 Function 区段必须放置在 Section 区段之后编写，以避免安装程序出现未可预知的问题。--#
Function .onInit
  nsProcess::_FindProcess "AiSound.exe"
  Pop $R0
  IntCmp $R0 0 running no_running no_running
  running:
  MessageBox MB_ICONQUESTION|MB_YESNO "安装程序检测到 ${PRODUCT_NAME} 正在运行，是否强行关闭并继续安装?" IDYES dokill IDNO stopit
  no_running:
  GoTo endding
  dokill:
  nsProcess::_KillProcess "AiSound.exe"
  Pop $R0
  GoTo endding
  stopit:
  Abort
  endding:
  nsProcess::_Unload
FunctionEnd

;开始卸载时检查：
Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "您确实要完全移除${PRODUCT_NAME} ，及其所有的组件？" IDYES probeProcess IDNO stopit
  probeProcess:
  nsProcess::_FindProcess "AiSound.exe"
  Pop $R0
  IntCmp $R0 0 running no_running no_running
  running:
  MessageBox MB_ICONQUESTION|MB_YESNO "安装程序检测到 ${PRODUCT_NAME} 正在运行，是否强行关闭并继续卸载?" IDYES dokill IDNO stopit
  no_running:
  GoTo endding
  dokill:
  nsProcess::_KillProcess "AiSound.exe"
  Pop $R0
  GoTo endding
  stopit:
  Abort
  endding:
  nsProcess::_Unload
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) 已成功地从您的计算机移除。"
FunctionEnd
