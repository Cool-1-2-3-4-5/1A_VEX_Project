# Clean VEX Build Script
Write-Host "Cleaning build files..." -ForegroundColor Yellow
cd "$PSScriptRoot\build"
Remove-Item *.bin, *.elf, *.map -Force -ErrorAction SilentlyContinue
Remove-Item src\*.o -Recurse -Force -ErrorAction SilentlyContinue
Write-Host "Build cleaned!" -ForegroundColor Green
Write-Host "Now BUILD and DOWNLOAD in VEXcode" -ForegroundColor Cyan
