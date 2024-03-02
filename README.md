# RemoteTTS
The software realises a text-to-speech application using the operating systems TTS features. The text is provided via a TCP/UDP interface.<br>
In addition, the software reports is state depending on the hosts power state, e.g. "bye-bye" when the host is goind to suspend or "alive" after it resumed to indicate the availability of the service. Therefore, `<ServerRunning>1</ServerRunning>` resp. `<ServerRunning>0</ServerRunning>` is send while 1 represents *service aliv*e and 0 indicates a *shutdown / bye-bye* of the service.

Using MS Windows, the program can detect resume and suspend events on its own an reports the status. This feature is not implemented for linux os.<br> 
Following a [StackOverFlow thread](https://stackoverflow.com/questions/23901419/how-to-capture-suspend-event-in-qt-linux) On linux you can put the following shell script in /etc/pm/sleep.d which executes a program with arguments. You can start a program and notify your main application in some way :
```
#!/bin/bash
case $1 in
suspend)
    #suspending to RAM
    /Path/to/Program/executable Sleeping
    ;;
resume)
    #resume from suspend
    sleep 3
    /Path/to/Program/executable Woken
    ;;
esac
```

The software provides a simple GUI for configuration.

A notification sound can be played prior to speak the submitted text (attention getter).

The project moved from [SourceForge](https://sourceforge.net/projects/remotetts/) to GitHub end of February 2024.

## Commandline Arguments

### Synopsis
```
remotetts [argument]
```

### Description

<p>*No argument* &emsp; Start with GUI</p>
<p>--alive &emsp; Sending *resumed* messag and exit</p>
<p>--byebye &emsp; Sendimng *going to suspend* message and exit.</p>

## IP-Interface
The software takes incoming ASCII strings, starting with a trigger word. 

Some special characters can be submitted using the HTML-Unicode replacement codes:
| HTML-Code | Special Character |
| --- | --- |
| `&#176;` | ° |
| `&#178;`| ² |
| `&#179;`| ³ |
| `&#246;`| ö |
| `&#252;`| ü |
| `&#196;`| Ä |
| `&#214;`| Ö |
| `&#220;`| Ü |
| `&#223;` | ß |

### Synopsis
```
[trigger][argument]
```

### Description

| Trigger | Argument | Example message | Note | 
| --- | --- | --- | --- |
| PLAY | WAV file to be played | `PLAYsound.wav` | The path to search for audio files can be configured. |
| SAY | Text to be saied | `SAYHello World`| All incomming txts are stored in a pipe. If the system receives a SAY command and is not already speaking / playing, an introsoud is put before the SAY-text. |


## Dvelopment Environment

* Qt 6.5.3 and MinGw

