# RemoteTTS
The software realises a text-to-speech application using the operating systems TTS features. The text is provided via a TCP/UDP interface.
In addition, the software reports is state depending on the hosts power state, e.g. "bye-bye" when the host is goind to suspend or "alive" after it resumed to indicate the availability of the service.

The software provides a simple GUI for configuration.

A notification sound can be played prior to speak the submitted text (attention getter).

The project moved from [SourceForge](https://sourceforge.net/projects/remotetts/) to GitHub end of February 2024.

## Interface
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
| SAY | Text to be saied | `SAYHello World`| - |

