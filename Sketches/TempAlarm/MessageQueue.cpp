#ifndef MESSAGE_QUEUE_CPP
#define MESSAGE_QUEUE_CPP

#include "Arduino.h"
#include "MessageQueue.h"

#define P Serial.print
#define PL Serial.println

CQElem::CQElem( message _msg ) : msg( _msg ), nextElem( 0 ) {}

void   CMessageQueue::AddElem( message _msg ) // Adds at the Tail of the Queue
{
  CQElem *elem = new CQElem( _msg );
  
  P(F("Elem created: ")); PL(int(elem));

  if (head==0) {
    P(F("Head set to element:")); PL(int(elem));
    head = elem;
  }
  if (tail!=0){
    P(("Added element to Tail:")); PL(int(elem));
    tail->nextElem = elem;
  };
  tail = elem;  
};

message CMessageQueue::RemoveElem( )
{
  message msg = NullMessage;
  CQElem *elemToDelete;
  if (head!=0){
    msg = head->msg;
    elemToDelete = head;
    head = head->nextElem;
    delete elemToDelete;
  };
  return msg;
};

void CMessageQueue::PrintQ(){
  message msg;
  CQElem *e;
  e = head;
  P(F("Message Queue: "));
  while (e!=0){
    P(int(e));  P(F(":")); P(e->msg); P(F(" | "));
    e = e->nextElem;
  };
  PL("");
};

int CMessageQueue::SizeOf(){
  CQElem *elem;
  int count = 0;
  elem = head;
  while (elem!=0){
    count += 1;
    elem = elem->nextElem;
  };
  return count;
}

#endif