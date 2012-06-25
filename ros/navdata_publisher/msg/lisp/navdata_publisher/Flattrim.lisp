; Auto-generated. Do not edit!


(in-package navdata_publisher-msg)


;//! \htmlinclude Flattrim.msg.html

(defclass <Flattrim> (ros-message)
  ((accs_offset
    :reader accs_offset-val
    :initarg :accs_offset
    :type (vector float)
   :initform (make-array 3 :element-type 'float :initial-element 0.0))
   (accs_gains
    :reader accs_gains-val
    :initarg :accs_gains
    :type (vector float)
   :initform (make-array 3 :element-type 'float :initial-element 0.0))
   (gyros_offset
    :reader gyros_offset-val
    :initarg :gyros_offset
    :type (vector float)
   :initform (make-array 3 :element-type 'float :initial-element 0.0))
   (gyros_gains
    :reader gyros_gains-val
    :initarg :gyros_gains
    :type (vector float)
   :initform (make-array 3 :element-type 'float :initial-element 0.0))
   (gyros110_offset
    :reader gyros110_offset-val
    :initarg :gyros110_offset
    :type (vector float)
   :initform (make-array 3 :element-type 'float :initial-element 0.0))
   (gyros110_gains
    :reader gyros110_gains-val
    :initarg :gyros110_gains
    :type (vector float)
   :initform (make-array 3 :element-type 'float :initial-element 0.0)))
)
(defmethod serialize ((msg <Flattrim>) ostream)
  "Serializes a message object of type '<Flattrim>"
    (map nil #'(lambda (ele) (let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream)))(slot-value msg 'accs_offset))
    (map nil #'(lambda (ele) (let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream)))(slot-value msg 'accs_gains))
    (map nil #'(lambda (ele) (let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream)))(slot-value msg 'gyros_offset))
    (map nil #'(lambda (ele) (let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream)))(slot-value msg 'gyros_gains))
    (map nil #'(lambda (ele) (let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream)))(slot-value msg 'gyros110_offset))
    (map nil #'(lambda (ele) (let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream)))(slot-value msg 'gyros110_gains))
)
(defmethod deserialize ((msg <Flattrim>) istream)
  "Deserializes a message object of type '<Flattrim>"
  (setf (slot-value msg 'accs_offset) (make-array 3))
  (let ((vals (slot-value msg 'accs_offset)))
    (dotimes (i 3)
(let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (aref vals i) (roslisp-utils:decode-single-float-bits bits)))))
  (setf (slot-value msg 'accs_gains) (make-array 3))
  (let ((vals (slot-value msg 'accs_gains)))
    (dotimes (i 3)
(let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (aref vals i) (roslisp-utils:decode-single-float-bits bits)))))
  (setf (slot-value msg 'gyros_offset) (make-array 3))
  (let ((vals (slot-value msg 'gyros_offset)))
    (dotimes (i 3)
(let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (aref vals i) (roslisp-utils:decode-single-float-bits bits)))))
  (setf (slot-value msg 'gyros_gains) (make-array 3))
  (let ((vals (slot-value msg 'gyros_gains)))
    (dotimes (i 3)
(let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (aref vals i) (roslisp-utils:decode-single-float-bits bits)))))
  (setf (slot-value msg 'gyros110_offset) (make-array 3))
  (let ((vals (slot-value msg 'gyros110_offset)))
    (dotimes (i 3)
(let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (aref vals i) (roslisp-utils:decode-single-float-bits bits)))))
  (setf (slot-value msg 'gyros110_gains) (make-array 3))
  (let ((vals (slot-value msg 'gyros110_gains)))
    (dotimes (i 3)
(let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (aref vals i) (roslisp-utils:decode-single-float-bits bits)))))
  msg
)
(defmethod ros-datatype ((msg (eql '<Flattrim>)))
  "Returns string type for a message object of type '<Flattrim>"
  "navdata_publisher/Flattrim")
(defmethod md5sum ((type (eql '<Flattrim>)))
  "Returns md5sum for a message object of type '<Flattrim>"
  "592e1961e0235e956ee622a1c9644431")
(defmethod message-definition ((type (eql '<Flattrim>)))
  "Returns full string definition for message of type '<Flattrim>"
  (format nil "float32[3] accs_offset     ~%float32[3] accs_gains~%float32[3] gyros_offset    ~%float32[3] gyros_gains ~%float32[3] gyros110_offset  ~%float32[3] gyros110_gains~%~%~%"))
(defmethod serialization-length ((msg <Flattrim>))
  (+ 0
     0 (reduce #'+ (slot-value msg 'accs_offset) :key #'(lambda (ele) (declare (ignorable ele)) (+ 4)))
     0 (reduce #'+ (slot-value msg 'accs_gains) :key #'(lambda (ele) (declare (ignorable ele)) (+ 4)))
     0 (reduce #'+ (slot-value msg 'gyros_offset) :key #'(lambda (ele) (declare (ignorable ele)) (+ 4)))
     0 (reduce #'+ (slot-value msg 'gyros_gains) :key #'(lambda (ele) (declare (ignorable ele)) (+ 4)))
     0 (reduce #'+ (slot-value msg 'gyros110_offset) :key #'(lambda (ele) (declare (ignorable ele)) (+ 4)))
     0 (reduce #'+ (slot-value msg 'gyros110_gains) :key #'(lambda (ele) (declare (ignorable ele)) (+ 4)))
))
(defmethod ros-message-to-list ((msg <Flattrim>))
  "Converts a ROS message object to a list"
  (list '<Flattrim>
    (cons ':accs_offset (accs_offset-val msg))
    (cons ':accs_gains (accs_gains-val msg))
    (cons ':gyros_offset (gyros_offset-val msg))
    (cons ':gyros_gains (gyros_gains-val msg))
    (cons ':gyros110_offset (gyros110_offset-val msg))
    (cons ':gyros110_gains (gyros110_gains-val msg))
))
