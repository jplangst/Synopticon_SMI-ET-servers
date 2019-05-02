cd ..\SMI_GlassesEyeTrackingServer\Release

start NewEyeTrackingServer.exe --iviewetg 127.0.0.1 --show-scene-h264-with-gaze --crossbarAddress 127.0.0.1:8080 --glassesID HFES

GOTO EndComments
::==============================Comments===========================================
OPTIONS

        --help               to print this information
        --show-eyes          to open two windows displaying the eye images
        --scale-eyes f       rescale eye images by multiplying image size by f
        --show-scene         to open a window displaying the scene images (the
                             scene video is decoded by the server and full image
                             will be transferred to the remote viewer)
        --show-scene-h264    to open a window displaying the scene images (the
                             scene video is transferred as H.264 stream,
                             decoded by the remote viewer)
        --show-scene-with-gaze  to open a window displaying the scene images (the
                             scene video is decoded by the server and already
                             contains the gaze cursor, full image is transferred to
                             the remote viewer)
        --show-scene-h264-with-gaze    to open a window displaying the scene images
                             (the scene video is transferred as H.264 stream,
                             decoded by the client and contains a gaze cursor
                             overlay created by the client)
        --scale-scene f      rescale scene images by multiplying image size by f
        --vrpn-port f        VRPN server port
        --server ip_address  connect to the specified remote iViewNG server,
                             the server can be setup (device parameters) by this
                             Tutorial-RemoteViewer
        --iviewetg ip_address  connect to the specified remote iViewNG server
                             which had already been setup by an iViewETG-Client.
                             This Tutorial-RemoteViewer will not submit any new device
                             parameters, it will only try to subscribe and listen to
                             the data it wants. Whether the data can be received
                             depends on the parameters that iViewETG-Client has set
        --calibrate1pt       to perform a 1-point calibration
        --calibrate3pt       to perform a 3-point calibration
        --samplingrate n     set eye tracking sampling rate to either 30 or 60Hz
        --scene30            scene 30hz
        --scene24            scene 24hz
                             explicit command; if not specified, server's behavior
                             defines how it will behave when this client leaves
::==============================End Comments===========================================
:EndComments