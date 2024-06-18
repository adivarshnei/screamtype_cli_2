#!/usr/bin/env python3

import sys
import os

import AppKit
import iterm2


async def main(connection):
    app = await iterm2.async_get_app(connection)

    await app.async_activate()
    await iterm2.Window.async_create(
        connection,
        command=f"/bin/zsh -c {os.path.dirname(os.path.realpath(__file__))}/{sys.argv[1]}",
    )

    os.system(
        """osascript &>/dev/null <<-EOF
            tell application "iTerm2"
                set bounds of front window to {20, 20, 500, 500}
            end tell
        EOF"""
    )


if __name__ == "__main__":
    AppKit.NSWorkspace.sharedWorkspace().launchApplication_("iTerm")
    iterm2.run_until_complete(main, True)
