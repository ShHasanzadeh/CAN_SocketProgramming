#include "socketcan.h"


/*
 * @brief   Constructor for SocketCAN class
 *
 */
SocketCAN::SocketCAN(QObject *parent)
    : QObject{parent}
{

}



/*
 * @brief   Destructor for SocketCAN class
 *          Close Both SocketCAN at Close App Position
 *
 */
SocketCAN::~SocketCAN() {


    if (socketCAN_OBD >= 0) {
        close(socketCAN_OBD);
        perror("Error ~SocketCAN ");
        std::cout << "~SocketCAN to kill CAN channel " << static_cast<int>(canChannel) << " " << socketCAN_OBD << " successfully!" << std::endl;


    }
}



/*
 * @brief   Send Method For emit CANChangeStatusSignal
 *          Able To Call in Other Classes
 *
 */
void SocketCAN::sendCANChangeStatusSignal(int canchannel, Status newstatus) {

    /* will use by  All Method of classes */
    emit CANChangeStatusSignal(canchannel, newstatus);

}



/*
 * @brief   To  Define & Init Both CAN0 CAN1 SocketCAN
 *          Call By Class Object Just For Once
 *
 */
void SocketCAN::canInit (int init_canChannel)
{


    emit CANChangeStatusSignal (init_canChannel , Status::CAN_INIT_START);

    /* Better to define in Local Scop */
    sockaddr_can addr;
    ifreq ifr;
    init_stopFlage=false;

    while (!init_stopFlage) {


        /* set zero addr structure */
        memset(&addr, 0, sizeof(addr));

        /* Define CAN Interface can0 or can1 */
        init_can_interface = (init_canChannel == 0) ? "can0" : "can1";


        /* Openning CAN Socket &&  USE SOCK_RAW to get newest Data From Socket */
        if ((socketCAN_OBD = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
            emit CANChangeStatusSignal (init_canChannel , Status::CAN_INIT_OPEN_SOCKET_FAILED);
            perror("Error while Openning CAN Socket ");
            exit(EXIT_FAILURE);
        }else{

            /* to get New Data FRom Cash Buffer */
            //setsockopt(socketCAN_OBD, SOL_CAN_RAW , CAN_RAW_RX_FILTER , &rcvbuf_size , sizeof(rcvbuf_size));

            /* setting CAN Socket Interface */
            strcpy(ifr.ifr_name, init_can_interface.c_str());
            if (ioctl(socketCAN_OBD, SIOCGIFINDEX, &ifr) < 0) {
                emit CANChangeStatusSignal (init_canChannel , Status::CAN_INIT_SETTING_SOCKET_FAILED);
                perror("Error while setting CAN interface ");
                std:: cerr << "Error while setting CAN interface " << init_can_interface << " " << socketCAN_OBD << std::endl;
                close(socketCAN_OBD);
                exit(EXIT_FAILURE);
            }else{

                /* Need To call Filter Exactly After Binding The Socket */
                canReadFilter(init_canChannel ,{} );
                addr.can_family = PF_CAN;
                addr.can_ifindex = ifr.ifr_ifindex;


                /* binding CAN Socket Interface */
                if (bind(socketCAN_OBD, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                    emit CANChangeStatusSignal (init_canChannel , Status::CAN_INIT_BINDING_SOCKET_FAILED);
                    perror("Error while binding CAN socket : ");
                    close(socketCAN_OBD);
                    exit(EXIT_FAILURE);
                }else {

                    /*  Add Filterbanks to the opened socketCAN */
                    /* Need To call Filter Exactly After Binding The Socket */
                    canReadFilter(init_canChannel ,{} );

                    /* set error mask */
                    can_err_mask_t err_mask = 0x00000FFFU;
                    setsockopt(socketCAN_OBD, SOL_CAN_RAW, CAN_RAW_ERR_FILTER,&err_mask, sizeof(err_mask));



                    /* set Read buffer Fram Size */
                    // setsockopt(socketCAN_OBD , SOL_SOCKET , SO_RCVBUF ,&read_msg_buffer_size , sizeof(read_msg_buffer_size));

                    /* set Write buffer Fram Size */
                    //int write_msg_buffer_size=12;
                    // setsockopt(socketCAN_OBD , SOL_SOCKET , SO_SNDBUF ,&write_msg_buffer_size , sizeof(write_msg_buffer_size));


                    /* Non-Blocking Mode    */
                    fcntl(socketCAN_OBD , F_SETFL , O_NONBLOCK);


                    std::cout << "CAN interface " << init_can_interface << " " << socketCAN_OBD << " initialized successfully!" << std::endl;


                    if (init_canChannel ==0 ){
                        socketCAN_OBD_can0 = getSocket();
                        init_stopFlage=true;
                        emit CANChangeStatusSignal (init_canChannel , Status::CAN_INIT_SUCCESS);
                        std::cout << "init_can_interface "<< init_can_interface <<" socketCAN_OBD_can0 : " << socketCAN_OBD_can0 << " get successfully!" << std::endl;
                    }else if (init_canChannel ==1 ){

                        socketCAN_OBD_can1 = getSocket();
                        init_stopFlage=true;
                        emit CANChangeStatusSignal (init_canChannel , Status::CAN_INIT_SUCCESS);
                        std::cout << "init_can_interface "<< init_can_interface << " socketCAN_OBD_can1 : " << socketCAN_OBD_can1 << " get successfully!" << std::endl;
                    }else {
                        emit CANChangeStatusSignal (init_canChannel , Status::CAN_INIT_NUM_CHANNEL_FAILED);
                        std::cout << " Your input init_can_interface "<< init_can_interface << " is not valid !!!!!!!!!!"<< std::endl;
                        init_stopFlage=true;

                    }


                }


            }

        }

    }

}



/*
 * @brief   return Socket Descriptor Value
 *          use in Init Can Method
 *
 */
int SocketCAN::getSocket() const {
    std::cout << "socketCAN_OBD_" << init_can_interface << " value is : " << socketCAN_OBD << std::endl;

    return socketCAN_OBD;
}





