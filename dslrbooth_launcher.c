#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>  // For OPENFILENAME
#include <shellapi.h> // For ShellExecute
#include <stdio.h>

#define DSLRBOOTH_PATH_KEY "Software\\YourAppName\\DSLRBoothPath"

// Function to get stored DSLRBooth path from the registry
void GetStoredDslrboothPath(char *path, size_t pathSize)
{
  HKEY hKey;
  if (RegOpenKeyEx(HKEY_CURRENT_USER, DSLRBOOTH_PATH_KEY, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
  {
    DWORD size = pathSize;
    RegQueryValueEx(hKey, "Path", NULL, NULL, (LPBYTE)path, &size);
    RegCloseKey(hKey);
  }
}

// Function to set the DSLRBooth path in the registry
void SetDslrboothPath(const char *path)
{
  HKEY hKey;
  if (RegCreateKeyEx(HKEY_CURRENT_USER, DSLRBOOTH_PATH_KEY, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS)
  {
    RegSetValueEx(hKey, "Path", 0, REG_SZ, (const BYTE *)path, strlen(path) + 1);
    RegCloseKey(hKey);
  }
}

// Function to show the custom message box
int ShowCustomMessageBox(const char *message, const char *title)
{
  return MessageBox(
      NULL,                                   // Handle to parent window
      message,                                // Message
      title,                                  // Title
      MB_YESNO | MB_ICONQUESTION | MB_TOPMOST // Type of message box (always on top)
  );
}

int main()
{
  char dslrboothPath[MAX_PATH] = "";
  GetStoredDslrboothPath(dslrboothPath, sizeof(dslrboothPath));

  if (strlen(dslrboothPath) == 0)
  {
    OPENFILENAME ofn;      // common dialog box structure
    char szFile[MAX_PATH]; // buffer for file name

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Executable Files (*.exe)\0*.exe\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
      // Store the selected path in the variable and registry
      strcpy(dslrboothPath, szFile);
      SetDslrboothPath(dslrboothPath);
    }
  }

  if (strlen(dslrboothPath) > 0)
  {
    ShellExecute(NULL, "open", dslrboothPath, NULL, NULL, SW_SHOWDEFAULT);

    const char *message = "Apakah proses setup DSLRBooth ini sudah selesai? \n\nKlik 'Ya' untuk membuka aplikasi pembayaran.\nKlik 'Tidak' untuk membatalkan setup.";
    const char *title = "Konfirmasi Setup DSLRBooth";

    int result = ShowCustomMessageBox(message, title);

    if (result == IDNO)
    {
      HWND hwnd = FindWindow(NULL, "DSLRBooth"); // Replace with actual window title
      if (hwnd != NULL)
      {
        SendMessage(hwnd, WM_CLOSE, 0, 0);
      }
      return 0;
    }

    // -- DO MAIN LOGIC HERE --

    // system("start startApplication.bat");
    ShellExecute(NULL, "open", "wscript.exe", "startApp.vbs", NULL, SW_HIDE);
  }

  return 0;
}
