 #!/usr/bin/env bash

 # Execute this file to install the taona cli tools into your path on OS X

 CURRENT_LOC="$( cd "$(dirname "$0")" ; pwd -P )"
 LOCATION=${CURRENT_LOC%Taona-Qt.app*}

 # Ensure that the directory to symlink to exists
 sudo mkdir -p /usr/local/bin

 # Create symlinks to the cli tools
 sudo ln -s ${LOCATION}/Taona-Qt.app/Contents/MacOS/taonad /usr/local/bin/taonad
 sudo ln -s ${LOCATION}/Taona-Qt.app/Contents/MacOS/taona-cli /usr/local/bin/taona-cli
