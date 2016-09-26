tell application "System Events"
    if ((get name of every process) contains "ScreenSaverEngine") then
        tell application "ScreenSaverEngine" to quit
        delay 0.5
        keystroke "<SENHA>"
        delay 0.5
        keystroke return
    end if
end tell
