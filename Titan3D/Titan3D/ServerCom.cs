using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace Titan3D
{
    class ServerCom
    {
        const int START = 1;
        const int STOP = 2;
        const int TEST = 3;

        private IPAddress ip_address;
        private int port;

        public ServerCom(byte[] ip_address, int port)
        {
            this.ip_address = new IPAddress(ip_address);
            this.port = port;
        }

        public void start(string filename, UInt32 width, UInt32 num_slices, UInt32 height)
        {
            Socket sender = new Socket(SocketType.Stream, ProtocolType.Tcp);
            sender.Connect(ip_address, port);
            string[] msgs = {START.ToString(), filename, height.ToString(),
                             num_slices.ToString(), width.ToString()};
            byte[] msg = Encoding.ASCII.GetBytes(String.Join(",", msgs) + "\0");
            int bytesSent = sender.Send(msg);
            sender.Shutdown(SocketShutdown.Both);
            sender.Close();
        }

        public void stop()
        {
            Socket sender = new Socket(SocketType.Stream, ProtocolType.Tcp);
            sender.Connect(ip_address, port);
            byte[] msg = Encoding.ASCII.GetBytes(STOP.ToString() + "\0");
            int bytesSent = sender.Send(msg);
            sender.Shutdown(SocketShutdown.Both);
            sender.Close();
        }
    }
}
