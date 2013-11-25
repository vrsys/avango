(require 'av-linux-event)

(add-device "mouse" "fpHIDInput")
(config-device "mouse" "tty" "/dev/input/event1")
(config-device "mouse" "vendor" "0x046D")
(config-device "mouse" "product" "0xC016")
(config-device "mouse" "version" "0x0340")
(config-device "mouse" "serial" "0x0")


(add-station "mouse" "mouse-station" 0)
(add-mapping "mouse" "mouse-station" EV_REL REL_X STATION_VALUE 0)
(add-mapping "mouse" "mouse-station" EV_REL REL_Y STATION_VALUE 1)

(define (dump-mouse)
  (display "mouse values: ")
  (display (get-values "mouse-station"))(newline)
  )

(start-device "mouse")
