


G_Q_angle=0
G_Q_gyro=0
G_R_angle=0
G_P_00=0
G_P_01=0
G_P_10=0
G_P_11=0
G_x_angle=0
G_x_bias=0

def b():
    global G_x_bias
    return G_x_bias


def ars_Init(Q_angle, Q_gyro, R_angle):
        global G_Q_angle,G_Q_gyro,G_R_angle
	G_Q_angle = Q_angle;
	G_Q_gyro  = Q_gyro;
	G_R_angle = R_angle;

def ars_predict(dotAngle, dt):
        global G_x_angle,G_x_bias,G_P_00,G_P_01,G_P_10,G_P_11,G_Q_gyro, G_Q_angle

	G_x_angle += dt * (dotAngle - G_x_bias);
	G_P_00 +=  - dt * (G_P_10 + G_P_01) + G_Q_angle * dt;
	G_P_01 +=  - dt * G_P_11;
	G_P_10 +=  - dt * G_P_11;
	G_P_11 +=  + G_Q_gyro * dt

#	print G_P_00, G_P_01, G_P_10, G_P_11


def ars_update(angle_m):
        global G_x_angle,G_x_bias,G_P_00,G_P_01,G_P_10,G_P_11,G_Q_gyro,G_R_angle

	y = angle_m - G_x_angle;
	
	S = G_P_00 + G_R_angle;
	K_0 = G_P_00 / S;
	K_1 = G_P_10 / S;
	
	G_x_angle +=  K_0 * y;
	G_x_bias  +=  K_1 * y;
#	print G_x_bias, K_1, y
#        print G_x_bias
	
	G_P_00 -= K_0 * G_P_00;
	G_P_01 -= K_0 * G_P_01;
	G_P_10 -= K_1 * G_P_00;
	G_P_11 -= K_1 * G_P_01;
	
	return G_x_angle;

