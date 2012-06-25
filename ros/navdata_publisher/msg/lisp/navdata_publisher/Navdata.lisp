; Auto-generated. Do not edit!


(in-package navdata_publisher-msg)


;//! \htmlinclude Navdata.msg.html

(defclass <Navdata> (ros-message)
  ((navboard_seq
    :reader navboard_seq-val
    :initarg :navboard_seq
    :type fixnum
    :initform 0)
   (acceleration
    :reader acceleration-val
    :initarg :acceleration
    :type (vector float)
   :initform (make-array 3 :element-type 'float :initial-element 0.0))
   (gyro
    :reader gyro-val
    :initarg :gyro
    :type (vector float)
   :initform (make-array 3 :element-type 'float :initial-element 0.0))
   (accelTemperature
    :reader accelTemperature-val
    :initarg :accelTemperature
    :type float
    :initform 0.0)
   (gyroTemperature
    :reader gyroTemperature-val
    :initarg :gyroTemperature
    :type float
    :initform 0.0)
   (vrefEpson
    :reader vrefEpson-val
    :initarg :vrefEpson
    :type float
    :initform 0.0)
   (vrefIDG
    :reader vrefIDG-val
    :initarg :vrefIDG
    :type float
    :initform 0.0)
   (height_us
    :reader height_us-val
    :initarg :height_us
    :type float
    :initform 0.0)
   (us_echo_start
    :reader us_echo_start-val
    :initarg :us_echo_start
    :type fixnum
    :initform 0)
   (us_echo_end
    :reader us_echo_end-val
    :initarg :us_echo_end
    :type fixnum
    :initform 0)
   (us_association_echo
    :reader us_association_echo-val
    :initarg :us_association_echo
    :type fixnum
    :initform 0)
   (us_distance_echo
    :reader us_distance_echo-val
    :initarg :us_distance_echo
    :type fixnum
    :initform 0)
   (us_courbe_temps
    :reader us_courbe_temps-val
    :initarg :us_courbe_temps
    :type fixnum
    :initform 0)
   (us_courbe_valeur
    :reader us_courbe_valeur-val
    :initarg :us_courbe_valeur
    :type fixnum
    :initform 0)
   (us_courbe_ref
    :reader us_courbe_ref-val
    :initarg :us_courbe_ref
    :type fixnum
    :initform 0)
   (newFloat
    :reader newFloat-val
    :initarg :newFloat
    :type (vector float)
   :initform (make-array 7 :element-type 'float :initial-element 0.0))
   (newUint16
    :reader newUint16-val
    :initarg :newUint16
    :type (vector fixnum)
   :initform (make-array 7 :element-type 'fixnum :initial-element 0)))
)
(defmethod serialize ((msg <Navdata>) ostream)
  "Serializes a message object of type '<Navdata>"
    (write-byte (ldb (byte 8 0) (slot-value msg 'navboard_seq)) ostream)
  (write-byte (ldb (byte 8 8) (slot-value msg 'navboard_seq)) ostream)
    (map nil #'(lambda (ele) (let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream)))(slot-value msg 'acceleration))
    (map nil #'(lambda (ele) (let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream)))(slot-value msg 'gyro))
  (let ((bits (roslisp-utils:encode-single-float-bits (slot-value msg 'accelTemperature))))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream))
  (let ((bits (roslisp-utils:encode-single-float-bits (slot-value msg 'gyroTemperature))))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream))
  (let ((bits (roslisp-utils:encode-single-float-bits (slot-value msg 'vrefEpson))))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream))
  (let ((bits (roslisp-utils:encode-single-float-bits (slot-value msg 'vrefIDG))))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream))
  (let ((bits (roslisp-utils:encode-single-float-bits (slot-value msg 'height_us))))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream))
    (write-byte (ldb (byte 8 0) (slot-value msg 'us_echo_start)) ostream)
  (write-byte (ldb (byte 8 8) (slot-value msg 'us_echo_start)) ostream)
    (write-byte (ldb (byte 8 0) (slot-value msg 'us_echo_end)) ostream)
  (write-byte (ldb (byte 8 8) (slot-value msg 'us_echo_end)) ostream)
    (write-byte (ldb (byte 8 0) (slot-value msg 'us_association_echo)) ostream)
  (write-byte (ldb (byte 8 8) (slot-value msg 'us_association_echo)) ostream)
    (write-byte (ldb (byte 8 0) (slot-value msg 'us_distance_echo)) ostream)
  (write-byte (ldb (byte 8 8) (slot-value msg 'us_distance_echo)) ostream)
    (write-byte (ldb (byte 8 0) (slot-value msg 'us_courbe_temps)) ostream)
  (write-byte (ldb (byte 8 8) (slot-value msg 'us_courbe_temps)) ostream)
    (write-byte (ldb (byte 8 0) (slot-value msg 'us_courbe_valeur)) ostream)
  (write-byte (ldb (byte 8 8) (slot-value msg 'us_courbe_valeur)) ostream)
    (write-byte (ldb (byte 8 0) (slot-value msg 'us_courbe_ref)) ostream)
  (write-byte (ldb (byte 8 8) (slot-value msg 'us_courbe_ref)) ostream)
    (map nil #'(lambda (ele) (let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream)))(slot-value msg 'newFloat))
    (map nil #'(lambda (ele)   (write-byte (ldb (byte 8 0) ele) ostream)
  (write-byte (ldb (byte 8 8) ele) ostream))(slot-value msg 'newUint16))
)
(defmethod deserialize ((msg <Navdata>) istream)
  "Deserializes a message object of type '<Navdata>"
  (setf (ldb (byte 8 0) (slot-value msg 'navboard_seq)) (read-byte istream))
  (setf (ldb (byte 8 8) (slot-value msg 'navboard_seq)) (read-byte istream))
  (setf (slot-value msg 'acceleration) (make-array 3))
  (let ((vals (slot-value msg 'acceleration)))
    (dotimes (i 3)
(let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (aref vals i) (roslisp-utils:decode-single-float-bits bits)))))
  (setf (slot-value msg 'gyro) (make-array 3))
  (let ((vals (slot-value msg 'gyro)))
    (dotimes (i 3)
(let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (aref vals i) (roslisp-utils:decode-single-float-bits bits)))))
  (let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (slot-value msg 'accelTemperature) (roslisp-utils:decode-single-float-bits bits)))
  (let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (slot-value msg 'gyroTemperature) (roslisp-utils:decode-single-float-bits bits)))
  (let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (slot-value msg 'vrefEpson) (roslisp-utils:decode-single-float-bits bits)))
  (let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (slot-value msg 'vrefIDG) (roslisp-utils:decode-single-float-bits bits)))
  (let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (slot-value msg 'height_us) (roslisp-utils:decode-single-float-bits bits)))
  (setf (ldb (byte 8 0) (slot-value msg 'us_echo_start)) (read-byte istream))
  (setf (ldb (byte 8 8) (slot-value msg 'us_echo_start)) (read-byte istream))
  (setf (ldb (byte 8 0) (slot-value msg 'us_echo_end)) (read-byte istream))
  (setf (ldb (byte 8 8) (slot-value msg 'us_echo_end)) (read-byte istream))
  (setf (ldb (byte 8 0) (slot-value msg 'us_association_echo)) (read-byte istream))
  (setf (ldb (byte 8 8) (slot-value msg 'us_association_echo)) (read-byte istream))
  (setf (ldb (byte 8 0) (slot-value msg 'us_distance_echo)) (read-byte istream))
  (setf (ldb (byte 8 8) (slot-value msg 'us_distance_echo)) (read-byte istream))
  (setf (ldb (byte 8 0) (slot-value msg 'us_courbe_temps)) (read-byte istream))
  (setf (ldb (byte 8 8) (slot-value msg 'us_courbe_temps)) (read-byte istream))
  (setf (ldb (byte 8 0) (slot-value msg 'us_courbe_valeur)) (read-byte istream))
  (setf (ldb (byte 8 8) (slot-value msg 'us_courbe_valeur)) (read-byte istream))
  (setf (ldb (byte 8 0) (slot-value msg 'us_courbe_ref)) (read-byte istream))
  (setf (ldb (byte 8 8) (slot-value msg 'us_courbe_ref)) (read-byte istream))
  (setf (slot-value msg 'newFloat) (make-array 7))
  (let ((vals (slot-value msg 'newFloat)))
    (dotimes (i 7)
(let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (aref vals i) (roslisp-utils:decode-single-float-bits bits)))))
  (setf (slot-value msg 'newUint16) (make-array 7))
  (let ((vals (slot-value msg 'newUint16)))
    (dotimes (i 7)
(setf (ldb (byte 8 0) (aref vals i)) (read-byte istream))
  (setf (ldb (byte 8 8) (aref vals i)) (read-byte istream))))
  msg
)
(defmethod ros-datatype ((msg (eql '<Navdata>)))
  "Returns string type for a message object of type '<Navdata>"
  "navdata_publisher/Navdata")
(defmethod md5sum ((type (eql '<Navdata>)))
  "Returns md5sum for a message object of type '<Navdata>"
  "5caf99a96dc76ca4a30c12a29967a314")
(defmethod message-definition ((type (eql '<Navdata>)))
  "Returns full string definition for message of type '<Navdata>"
  (format nil "uint16 navboard_seq~%float32[3] acceleration~%float32[3] gyro~%float32 accelTemperature~%float32 gyroTemperature~%float32 vrefEpson~%float32 vrefIDG~%float32 height_us~%uint16 us_echo_start~%uint16 us_echo_end~%uint16 us_association_echo~%uint16 us_distance_echo~%uint16 us_courbe_temps~%uint16 us_courbe_valeur~%uint16 us_courbe_ref~%float32[7] newFloat~%uint16[7] newUint16~%~%~%~%~%"))
(defmethod serialization-length ((msg <Navdata>))
  (+ 0
     2
     0 (reduce #'+ (slot-value msg 'acceleration) :key #'(lambda (ele) (declare (ignorable ele)) (+ 4)))
     0 (reduce #'+ (slot-value msg 'gyro) :key #'(lambda (ele) (declare (ignorable ele)) (+ 4)))
     4
     4
     4
     4
     4
     2
     2
     2
     2
     2
     2
     2
     0 (reduce #'+ (slot-value msg 'newFloat) :key #'(lambda (ele) (declare (ignorable ele)) (+ 4)))
     0 (reduce #'+ (slot-value msg 'newUint16) :key #'(lambda (ele) (declare (ignorable ele)) (+ 2)))
))
(defmethod ros-message-to-list ((msg <Navdata>))
  "Converts a ROS message object to a list"
  (list '<Navdata>
    (cons ':navboard_seq (navboard_seq-val msg))
    (cons ':acceleration (acceleration-val msg))
    (cons ':gyro (gyro-val msg))
    (cons ':accelTemperature (accelTemperature-val msg))
    (cons ':gyroTemperature (gyroTemperature-val msg))
    (cons ':vrefEpson (vrefEpson-val msg))
    (cons ':vrefIDG (vrefIDG-val msg))
    (cons ':height_us (height_us-val msg))
    (cons ':us_echo_start (us_echo_start-val msg))
    (cons ':us_echo_end (us_echo_end-val msg))
    (cons ':us_association_echo (us_association_echo-val msg))
    (cons ':us_distance_echo (us_distance_echo-val msg))
    (cons ':us_courbe_temps (us_courbe_temps-val msg))
    (cons ':us_courbe_valeur (us_courbe_valeur-val msg))
    (cons ':us_courbe_ref (us_courbe_ref-val msg))
    (cons ':newFloat (newFloat-val msg))
    (cons ':newUint16 (newUint16-val msg))
))
