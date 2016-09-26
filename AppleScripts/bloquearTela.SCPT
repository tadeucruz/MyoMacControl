tell application "System Events"
	tell security preferences
		set require password to wake to true
	end tell
end tell
tell application "System Events"
    if ((get name of every process) does not contain "ScreenSaverEngine") then
        set ss to screen saver "Flurry"
        start ss
    end if
end tell
