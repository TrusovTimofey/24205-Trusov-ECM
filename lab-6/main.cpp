#include <iostream>
#include <map>
#include <libusb-1.0/libusb.h>

using namespace std;

void printDev(libusb_device *dev);
void printVendor(libusb_device *dev, libusb_device_handle *handle, libusb_device_descriptor &desc);
void printProduct(libusb_device *dev, libusb_device_handle *handle, libusb_device_descriptor &desc);
void printSerialNumber(libusb_device *dev, libusb_device_handle *handle, libusb_device_descriptor &desc);

const map<uint8_t, string> devClasses = {
    {0x00, "Код отсутствует"},
    {0x01, "Аудиоустройство"},
    {0x02, "Коммуникационное устройство"},
    {0x03, "Устройство пользовательского интерфейса"},
    {0x05, "Физическое устройство"},
    {0x06, "Изображения"},
    {0x07, "Принтер"},
    {0x08, "Устройство хранения данных"},
    {0x09, "Концентратор"},
    {0x0A, "CDC-Data"},
    {0x0B, "Smart Card"},
    {0x0D, "Content Security"},
    {0x0E, "Видеоустройство"},
    {0x0F, "Персональное медицинское устройство"},
    {0x10, "Аудио- и видеоустройства"},
    {0xDC, "Диагностическое устройство"},
    {0xE0, "Беспроводный контроллер"},
    {0xEF, "Различные устройства"},
    {0xFE, "Специфическое устройство"}
};

const map<uint16_t, string> devVendors = {
    {0x2b7e, "Sony"},
    {0x8086, "Intel"},
    {0x8087, "Intel"},
    {0x04f2, "Chicony"},
    {0x1d6b, "Linux Foundation"},
    {0x046d, "Logitech"},
    {0x045e, "Microsoft"},
    {0x0bda, "Realtek"},
    {0x0781, "SanDisk"},
    {0x0951, "Kingston"},
    {0x413c, "Dell"},
    {0x0489, "Foxconn"},
    {0x04e8, "Samsung"},
    {0x0cf3, "Qualcomm"},
    {0x0a5c, "Broadcom"},
    {0x1050, "Yubico"},
    {0x18d1, "Google"}

};

const map<uint16_t, map<uint16_t, string>> devProducts = {
    {0x8086,
        {{0x51ed, "USB Hub"},
         {0x461e, "USB Controller"},
         {0x02e8, "Thunderbolt"},
         {0x0aa7, "Wireless Hub"}}
    },
    {0x8087,
        {{0x0026, "Bluetooth Controller"},
         {0x0033, "Wireless Controller"},
         {0x07dc, "Wireless Adapter"},
         {0x0a2b, "Bluetooth Radio"}}
    },
    {0x2b7e,
        {{0xb685, "USB Composite Device"},
         {0x0900, "PlayStation Controller"},
         {0x0901, "DualShock Controller"}}
    },
    {0x04f2,
        {{0x0729, "USB Camera"},
         {0xb5d9, "Webcam"},
         {0xb6a5, "Keyboard"}}
    },
    {0x1d6b,
        {{0x0002, "USB Hub"},
         {0x0003, "USB Host Controller"},
         {0x0001, "USB Device"}}
    },
    {0x046d,
        {{0xc52b, "Unifying Receiver"},
         {0xc539, "Keyboard"},
         {0xc07d, "Mouse"}}
    }
};

int main(){
    libusb_context* ctx = nullptr;
    int r = libusb_init(&ctx); // инициализировать библиотеку libusb
    if(r < 0){ 
        cerr << "Ошибка: инициализация не выполнена, код: " << r << endl;
        return 1;
    }

    libusb_set_debug(ctx, 3); // задать уровень подробности отладочных сообщений

    libusb_device **devs; // указатель на указатель на устройство,
    auto devsCount = libusb_get_device_list(ctx, &devs);     // получить список всех найденных USB- устройств
    if(devsCount < 0){
        cerr << "Ошибка: список USB устройств не получен. " << r << endl;
        return 1;
    }

    cout << "Найдено устройств:" << devsCount << endl;

    for(ssize_t i = 0; i < devsCount; i++) {
        cout << "===========================================================" << endl;
        cout << "Устройство " << (i+1) << endl;
        printDev(devs[i]); // печать параметров устройства
        cout << "===========================================================" << endl;
    }

    libusb_free_device_list(devs, 1); // освободить память, выделенную функцией получения списка устройств
    libusb_exit(ctx); // завершить работу с библиотекой libusb,
    return 0;
}

void printDev(libusb_device *dev)
{
    libusb_device_descriptor desc; // дескриптор устройства
    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0)
    {
        cerr << "Ошибка: дескриптор устройства не получен, код: " << r << endl;
        return;
    }

    if(devClasses.count(desc.bDeviceClass)) cout << "Класс устройства: " << devClasses.at(desc.bDeviceClass) << endl;
    else cout << "Неизвестный класс устройства" << endl;

    libusb_device_handle *handle = NULL;
    libusb_open(dev, &handle);

    printVendor(dev, handle, desc);
    printProduct(dev, handle, desc);
    printSerialNumber(dev, handle, desc);

    if(handle)libusb_close(handle);
}

void printVendor(libusb_device *dev, libusb_device_handle *handle, libusb_device_descriptor &desc)
{
    if(handle){
        unsigned char vendor[256];
        if (libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, vendor, sizeof(vendor)) >= 0){
            cout << "Производитель устройства: " << vendor << endl; 
            return;
        } 
    }

    if(devVendors.count(desc.idVendor)) cout << "Производитель устройства: " << devVendors.at(desc.idVendor) << endl;
    else cout << "Неизвестный производитель устройства" << endl;
    
}

void printProduct(libusb_device *dev, libusb_device_handle *handle, libusb_device_descriptor &desc)
{
    if(handle){
        unsigned char product[256];
        libusb_get_string_descriptor_ascii(handle, desc.iProduct, product, sizeof(product));
        if (libusb_get_string_descriptor_ascii(handle, desc.iProduct, product, sizeof(product)) >= 0){
            cout << "Название изделия: " << product << endl;
            return;
        } 
    }

    if(devProducts.count(desc.idVendor) && devProducts.at(desc.idVendor).count(desc.idProduct)) 
        cout << "Название изделия: " << devProducts.at(desc.idVendor).at(desc.idProduct) << endl;
    else cout << "Неизвестное изделие" << endl;
      
}

void printSerialNumber(libusb_device *dev, libusb_device_handle *handle, libusb_device_descriptor &desc)
{
    if(!handle) return;
    unsigned char serial[256];
    int r = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, serial, sizeof(serial));
    if (r < 0) cerr << "Ошибка: не удалось получить серийный номер, код: " << r << endl;
    else cout << "Серийный номер: " << serial << endl;

}