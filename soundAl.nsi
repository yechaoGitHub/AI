; �ýű�ʹ�� HM VNISEdit �ű��༭���򵼲���

; ��װ�����ʼ���峣��
!define PRODUCT_NAME "AiSound"
!define PRODUCT_VERSION "1.0.1"
!define PRODUCT_PUBLISHER "AiSound"
!define PRODUCT_WEB_SITE "http://www.aisound.com"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\AiSound.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma

; ------ MUI �ִ����涨�� (1.67 �汾���ϼ���) ------
!include "MUI.nsh"

; MUI Ԥ���峣��
!define MUI_ABORTWARNING
!define MUI_ICON "icon.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"


; ��ӭҳ��
!insertmacro MUI_PAGE_WELCOME
; ���Э��ҳ��
; ��װ����ҳ��
!insertmacro MUI_PAGE_INSTFILES
; ��װ���ҳ��
!define MUI_FINISHPAGE_RUN "$INSTDIR\AiSound.exe"
!insertmacro MUI_PAGE_FINISH

; ��װж�ع���ҳ��
!insertmacro MUI_UNPAGE_INSTFILES


; ��װ�����������������
!insertmacro MUI_LANGUAGE "English"


; ��װԤ�ͷ��ļ�
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI �ִ����涨����� ------

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
 *  �����ǰ�װ�����ж�ز���  *
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

#-- ���� NSIS �ű��༭�������� Function ���α�������� Section ����֮���д���Ա��ⰲװ�������δ��Ԥ֪�����⡣--#
Function .onInit
  nsProcess::_FindProcess "AiSound.exe"
  Pop $R0
  IntCmp $R0 0 running no_running no_running
  running:
  MessageBox MB_ICONQUESTION|MB_YESNO "��װ�����⵽ ${PRODUCT_NAME} �������У��Ƿ�ǿ�йرղ�������װ?" IDYES dokill IDNO stopit
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

;��ʼж��ʱ��飺
Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "��ȷʵҪ��ȫ�Ƴ�${PRODUCT_NAME} ���������е������" IDYES probeProcess IDNO stopit
  probeProcess:
  nsProcess::_FindProcess "AiSound.exe"
  Pop $R0
  IntCmp $R0 0 running no_running no_running
  running:
  MessageBox MB_ICONQUESTION|MB_YESNO "��װ�����⵽ ${PRODUCT_NAME} �������У��Ƿ�ǿ�йرղ�����ж��?" IDYES dokill IDNO stopit
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
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) �ѳɹ��ش����ļ�����Ƴ���"
FunctionEnd
