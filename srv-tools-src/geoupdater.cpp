#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv)
{
    system("wget http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz -O /var/tmp/GeoIP.dat.gz"
           " && cd /var/tmp/"
           " && gzip --uncompress GeoIP.dat.gz"
           " && rm -rf /usr/local/share/GeoIP/GeoIP.dat"
           " && mv -f /var/tmp/GeoIP.dat /usr/local/share/GeoIP/");

    system("wget http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz -O /var/tmp/GeoLiteCity.dat.gz"
           " && cd /var/tmp/"
           " && gzip --uncompress GeoLiteCity.dat.gz"
           " && rm -rf /usr/local/share/GeoIP/GeoLiteCity.dat"
           " && mv -f /var/tmp/GeoLiteCity.dat /usr/local/share/GeoIP/");

    return EXIT_SUCCESS;
}

