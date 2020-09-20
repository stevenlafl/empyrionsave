![](https://user-images.githubusercontent.com/2539092/93069610-e00ab680-f64b-11ea-8629-1ba739279805.png)

# Usage

It's pretty simple. Select the folder (the "Save Location") you wish you save backups of, and where (the "Backup Location") you wish to place those backups (renamed to `<folder name>_<time code>` or `<folder name>_<time code>.zip` respectively depending on the checkbox). Slide the timer for how often you want to backup. Hit Start. Optionally save your settings with the Save Settings button for next time.

# Building

Grab the latest release from https://github.com/stachenov/quazip and stick it into a new directory at the project root called "thirdparty".

Example: `thirdparty/quazip-0.9.1`

Now edit the empyrionsave.pro file and change the line `include(thirdparty/quazip-0.9.1/quazip.pri)` with whatever the new location like above.

Build it with Qt Creator

# Disclaimer

I am not affiliated in any way with *[Empyrion - Galactic Survival](https://empyriongame.com/)* or Eleon Game Studios nor do I have any rights to the name. This is so-called because it was inspired by *Empyrion*, and written for *Empyrion*. It can however be used to store backups of anything. I highly recommend *Empyrion* to anyone who lives survival games. If requested, I will change the name.

# License

This software is provided free and open source under the GPL v3 license. See more in LICENSE.
