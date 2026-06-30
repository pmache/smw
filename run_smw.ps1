$env:Path = "C:\msys64\mingw64\bin;$env:Path"
Push-Location "$PSScriptRoot"
try {
    .\build\smw.exe
} finally {
    Pop-Location
}
