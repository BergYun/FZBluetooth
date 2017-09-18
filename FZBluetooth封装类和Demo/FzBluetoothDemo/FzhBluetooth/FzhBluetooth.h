//
//  FzhBluetooth.h
//  BlueTooth
//
//  Created by 付正 on 2017/9/14.
//  Copyright © 2017年 BFMobile. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

/** 蓝牙状态改变的block */
typedef void(^FZStateUpdateBlock)(CBCentralManager *central);
/** 发现一个蓝牙外设的block */
typedef void(^FZDiscoverPeripheralBlock)(CBCentralManager *central, CBPeripheral *peripheral, NSDictionary *advertisementData, NSNumber *RSSI);
/** 连接完成的block*/
typedef void(^FZConnectSuccessBlock)(CBPeripheral *peripheral,CBService *service, CBCharacteristic *character);
/** 连接失败的block*/
typedef void(^FZConnectFailBlock)(CBPeripheral *peripheral, NSError *error);
/** 获取蓝牙外设信号的回调 */
typedef void(^FZGetRSSIBlock)(double number, NSError *error);
/** 往特性中写入数据的回调 */
typedef void(^FZWriteToCharacteristicBlock)(CBCharacteristic *characteristic, NSError *error);
/** 设备返回数据的回调 */
typedef void(^FZEquipmentReturnBlock)(CBPeripheral *peripheral, CBCharacteristic *characteristic, NSString *returnStr, NSError *error);


@interface FzhBluetooth : NSObject<CBPeripheralDelegate,CBCentralManagerDelegate,CBPeripheralManagerDelegate>

@property (copy, nonatomic) FZStateUpdateBlock stateUpdateBlock;
/** 发现一个蓝牙外设的回调 */
@property (copy, nonatomic) FZDiscoverPeripheralBlock               discoverPeripheralBlcok;
/** 连接外设完成的回调 */
@property (copy, nonatomic) FZConnectSuccessBlock                connectSuccessBlock;
/** 连接外设失败的回调 */
@property (copy, nonatomic) FZConnectFailBlock                connectFailBlock;
/** 获取蓝牙外设信号强度的回调  */
@property (copy, nonatomic) FZGetRSSIBlock                          getRSSIBlock;
/** 将数据写入特性中的回调 */
@property (copy, nonatomic) FZWriteToCharacteristicBlock            writeToCharacteristicBlock;
@property (copy, nonatomic) FZEquipmentReturnBlock equipmentReturnBlock;

@property (nonatomic, strong) CBCharacteristic * writeCharacteristic;//可写入参数,蓝牙连接成功,2秒后获得参数值
@property (nonatomic, strong) CBCharacteristic * notifyCharacteristic;//可通知参数,蓝牙连接成功,2秒后获得参数值

+(instancetype)shareInstance;

/**
 设置设备特性UUID，如果不设置默认为空
 */
@property (nonatomic, strong) NSString *UUIDString;

/**
 *  开始搜索蓝牙外设，每次在block中返回一个蓝牙外设信息
 *
 *  @param nameStr  模糊搜索设备名称，目标设备名称包含字段
 *  返回的block参数可参考CBCentralManager 的 centralManager:didDiscoverPeripheral:advertisementData:RSSI:
 *
 *  @param discoverBlock 搜索到蓝牙外设后的回调
 */
- (void)scanForPeripheralsWithPrefixName:(NSString *)nameStr discoverPeripheral:(FZDiscoverPeripheralBlock)discoverBlock;

/**
 *  连接某个蓝牙外设，并查询服务，特性，特性描述
 *
 *  @param peripheral          要连接的蓝牙外设
 *  @param completionBlock     操作执行完的回调
 */
- (void)connectPeripheral:(CBPeripheral *)peripheral
            completeBlock:(FZConnectSuccessBlock)completionBlock
                failBlock:(FZConnectFailBlock)failBlock;

/**
 *  获取某外设的距离
 *
 *  @param getRSSIBlock 获取信号完成后的回调
 */
- (void)readRSSICompletionBlock:(FZGetRSSIBlock)getRSSIBlock;

/**
 RSSI转距离number

 @param RSSI RSSI
 @return 距离
 */
- (double)fzRssiToNumber:(NSNumber *)RSSI;

/**
 *  往某个特性中写入数据
 *
 *  @param dataStr       写入的数据
 *  @param characteristic 特性对象
 *  @param completionBlock 写入完成后的回调,只有type为CBCharacteristicWriteWithResponse时，才会回调
 */
- (void)writeValue:(NSString *)dataStr forCharacteristic:(CBCharacteristic *)characteristic completionBlock:(FZWriteToCharacteristicBlock)completionBlock returnBlock:(FZEquipmentReturnBlock)equipmentBlock;

/**
 *  停止扫描
 */
- (void)stopScan;

/**
 *  断开蓝牙连接
 */
- (void)cancelPeripheralConnection;

/**
 NSData转16进制NSString

 @param data data数据
 @return string数据
 */
- (NSString *)fzHexStringFromData:(NSData *)data;

/**
 NSString转NSData

 @param str string数据
 @return data数据
 */
- (NSMutableData *)convertHexStrToData:(NSString *)str;

@end
