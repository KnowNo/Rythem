#include "pipedata.h"
#include <QDebug>
#include <QStringList>

PipeData::PipeData(int socketDescriptor):socketId(socketDescriptor){
}


void PipeData::setRequestHeader(QByteArray theHeader){
    QByteArray header = theHeader;
    header.replace("\r\n","\n");
    int i=0,l=header.length();
    //firstline
    i=header.indexOf('\n');
    Q_ASSERT(i!=-1);
    QByteArray firstLine = header.left(i).simplified();
    QList<QByteArray> sigs = firstLine.split(' ');
    Q_ASSERT(sigs.length() == 3);
    requestMethod = sigs.at(0);
    fullUrl = sigs.at(1);
    protocol = sigs.at(2);

    // change http://aaa.com/a/b/c?d to /a/b/c?d
    path = "/";
    int n;
    qDebug()<<"fullUrl="<<fullUrl;
    if(fullUrl.indexOf("://")!=-1){
        n = fullUrl.split("://")[1].indexOf("/");
        if(n!=-1 && n<fullUrl.length()-1){
            path = fullUrl.split("://")[1].mid(n);
        }
    }else{
        n = fullUrl.indexOf("/");
        if(n!=-1 && n<fullUrl.length()-1){
            path = fullUrl.mid(n);
        }
    }

    //the rest..
    while(i<l){
        int j=header.indexOf('\n');
        if(j==-1){// last line
            j=l;
        }
        QByteArray line = header.mid(i,j-i);
        qDebug()<<line;

        int splitIndex = line.indexOf(':');
        QByteArray name = QByteArray(line.left(splitIndex));
        QByteArray value = QByteArray(line.mid(splitIndex+1).trimmed());
        //setRequestHeader(name,value);
        allRequestHeaders[name]=value;
        //setRequestHeader(name,value);
        i=j+1;
    }
}


QByteArray PipeData::getResponseHeader(QByteArray name)const{
    return QByteArray();
}
QByteArray PipeData::getResponseHeader()const{

}
QByteArray PipeData::getResponseBody()const{

}

QByteArray PipeData::getRequestHeader() const{
    if(requestRawData.isEmpty()){
        return requestRawData;
    }
    int i = requestRawData.indexOf("\r\n\r\n");
    if(i==-1){
        i = requestRawData.indexOf("\r\n");
    }
    if(i==-1){
        return requestRawData;
    }else{
        return requestRawData.left(i);
    }
}
QByteArray PipeData::getRequestHeader(QByteArray name) const{
    return QByteArray();
}
QByteArray PipeData::getRequestBody()const{
    if(requestRawData.isEmpty()){
        return QByteArray();
    }
    int i = requestRawData.indexOf("\r\n\r\n");
    if(i!=-1){
        return requestRawData.mid(i+4);
    }else{
        i = requestRawData.indexOf("\r\n");
        if(i!=-1){
            return requestRawData.mid(i+2);
        }
    }
    return QByteArray();

}



void PipeData::setRequestRawData(QByteArray request){//only for copy Ctor

}
void PipeData::setResponseRawData(QByteArray response){//only for copy Ctor

}

