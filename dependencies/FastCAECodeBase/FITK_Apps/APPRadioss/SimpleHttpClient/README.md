# SimpleHttpClient

A minimal Qt Widgets HTTP client for communicating with `FastCAERadiossServer`.

## Build (VS2017)

1. Run `Create_Radioss_Sln.bat` at workspace root.
2. Open `APPRadioss.sln` in Visual Studio 2017.
3. Build project `SimpleHttpClient` with configuration `Debug|x64`.

Output executable:
- `SimpleHttpClient/debug/SimpleHttpClient.exe`

## Usage

1. Start `FastCAERadiossServer` first.
2. Launch `SimpleHttpClient.exe`.
3. Default URL is `http://127.0.0.1:8888/`.
4. Choose `GET` or `POST`.
5. For `POST`, fill request body in the text box.
6. Click `Send` and inspect status code and response body in the response panel.

## Notes

- Server config comes from `FastCAERadiossServer/FITKServer.ini`.
- If your server port changes, update URL in the client UI.
