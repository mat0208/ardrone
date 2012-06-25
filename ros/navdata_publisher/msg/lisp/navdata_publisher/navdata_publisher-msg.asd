
(in-package :asdf)

(defsystem "navdata_publisher-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils)
  :components ((:file "_package")
    (:file "Navdata" :depends-on ("_package"))
    (:file "_package_Navdata" :depends-on ("_package"))
    (:file "Flattrim" :depends-on ("_package"))
    (:file "_package_Flattrim" :depends-on ("_package"))
    ))
