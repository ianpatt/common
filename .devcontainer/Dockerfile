FROM ubuntu:24.04

RUN apt-get update && \
    apt-get install -y wine64 python3 msitools ca-certificates && \
    apt-get clean -y && \
    rm -rf /var/lib/apt/lists/*

# Initialize the wine environment. Wait until the wineserver process has
# exited before closing the session, to avoid corrupting the wine prefix.
RUN $(command -v wine64 || command -v wine || false) wineboot --init && \
    while pgrep wineserver > /dev/null; do sleep 1; done

WORKDIR /opt/msvc

COPY msvc-wine/lowercase msvc-wine/fixinclude msvc-wine/install.sh msvc-wine/vsdownload.py msvc-wine/msvctricks.cpp msvc-wine/msvcenv-native.sh  ./
COPY msvc-wine/wrappers/* ./wrappers/

RUN PYTHONUNBUFFERED=1 ./vsdownload.py --accept-license --dest /opt/msvc && \
    ./install.sh /opt/msvc && \
    rm lowercase fixinclude install.sh vsdownload.py && \
    rm -rf wrappers

COPY --chmod=755 activate-x86.sh ./bin/x86/activate.sh
COPY --chmod=755 build.sh ./build.sh

RUN apt-get update && \
    apt-get install -y git-all cmake && \
    apt-get clean -y && \
    rm -rf /var/lib/apt/lists/* && \
    rm -rf build extern && \
    echo "Activate the x86 MSVC environment with 'source /opt/msvc/bin/x86/activate.sh' before using MSVC tools like cl.exe" && \
    echo "Note that the wine server must be running for MSVC tools to work, so make sure to initialize the wine environment with 'wineboot --init' before activating the MSVC environment"

# Later stages which actually uses MSVC can ideally start a persistent
# wine server like this:
#RUN wineserver -p && \
#    $(command -v wine64 || command -v wine || false) wineboot && \
