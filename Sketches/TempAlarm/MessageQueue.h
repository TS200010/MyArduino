#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

enum message { TA_NullMessage, TA_Button1Click, TA_Button2Click, TA_ReadAndProcess };

struct CQElem
{
  CQElem( message _msg );
  message msg;
  CQElem *nextElem; 
};


class CMessageQueue
{
public:
    void    AddElem( message _msg ); // Adds at the Tail of the Queue
    message RemoveElem();
    void    PrintQ();
    int     SizeOf();

private:
    CQElem *head = 0;
    CQElem *tail = 0;

};

#endif
