# Define variables
$TargetFolder = "app"
$Executable = "..\..\build\bin\Release\Release\3DViewer.exe"
$Resources = "..\..\res"
$ImguiConfig = "..\..\res\imgui.ini"
$LicenseFile = "..\..\LICENSE"

# Create the target folder if it doesn't exist
if (-not (Test-Path -Path $TargetFolder)) {
    New-Item -ItemType Directory -Path $TargetFolder | Out-Null
}

# Copy the executable file to the target folder
if (Test-Path -Path $Executable) {
    Copy-Item -Path $Executable -Destination $TargetFolder -Force
} else {
    Write-Host "Warning: Binary file '$Executable' does not exist. Skipping."
}

# Copy the IMGUI configuration file to the target folder
if (Test-Path -Path $ImguiConfig) {
    Copy-Item -Path $ImguiConfig -Destination $TargetFolder -Force
} else {
    Write-Host "Warning: IMGUI configuration file '$ImguiConfig' does not exist. Skipping."
}

# Copy the resources folder to the target folder
if (Test-Path -Path $Resources) {
    Copy-Item -Path $Resources -Destination $TargetFolder -Recurse -Force
} else {
    Write-Host "Warning: Resources folder '$Resources' does not exist. Skipping."
}

# Copy the LICENSE file to the target folder
if (Test-Path -Path $LicenseFile) {
    Copy-Item -Path $LicenseFile -Destination $TargetFolder -Force
} else {
    Write-Host "Warning: LICENSE file '$LicenseFile' does not exist. Skipping."
}

 & "iscc.exe" "3DViewer_Setup_Script.iss"