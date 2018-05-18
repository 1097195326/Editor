// Fill out your copyright notice in the Description page of Project Settings.

#include "WebViewViewController.h"

#if PLATFORM_IOS

#import "WebViewJavascriptBridge.h"
#import <AVFoundation/AVFoundation.h>
#import <AssetsLibrary/AssetsLibrary.h>
#import <AlipaySDK/AlipaySDK.h>
#import <sys/utsname.h>
#import <UIKit/UIKit.h>

#define statusBar_HEIGHT  [[UIApplication sharedApplication]statusBarFrame].size.height

@interface WebViewViewController ()<UIWebViewDelegate,UIImagePickerControllerDelegate,UINavigationControllerDelegate>

@property (strong, nonatomic) UIWebView *webView;

@property (strong, nonatomic) UILabel *titleLabel;

@property (nonatomic, strong) WebViewJavascriptBridge* bridge;

@property (copy, nonatomic) NSMutableString *loadedURL;

@end

@implementation WebViewViewController

- (void)viewDidLoad {
    
    [super viewDidLoad];
    
    struct utsname systemInfo;
    uname(&systemInfo);
    
    UIView *view = nil;
    
    //iPhone
    if ([[NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding] isEqualToString:@"iPhone10,3"] || [[NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding] isEqualToString:@"iPhone10,6"])
    {
        UIView *topView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width, 44)];
        
        topView.backgroundColor = [UIColor blackColor];
        
        [self.view addSubview: topView];
        
        view = [[UIView alloc] initWithFrame:CGRectMake(0, CGRectGetMaxY(topView.frame), [UIScreen mainScreen].bounds.size.width, 44)];
        
    }
    else
    {
        view = [[UIView alloc] initWithFrame:CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width, statusBar_HEIGHT + 44)];
        
    }
    
    view.backgroundColor = [UIColor blackColor];
    
    [self.view insertSubview:view atIndex:0];
    
    self.view.backgroundColor = [UIColor blackColor];
    
    UIButton *button = [UIButton buttonWithType:(UIButtonTypeSystem)];
    
    button.frame = CGRectMake(15, 13, 22, 22);
    
    button.titleLabel.font = [UIFont systemFontOfSize:14];
    
    NSString *bundlePath = [[NSBundle mainBundle] pathForResource:@"AlipaySDK" ofType: @"bundle"];
    NSString *imagePath = [bundlePath stringByAppendingPathComponent :self.imageName];
    
    NSLog(@"imagePath:%@:%@",imagePath,[UIImage imageWithContentsOfFile: imagePath]);
    
    [button setBackgroundImage:[UIImage imageWithContentsOfFile: imagePath] forState:(UIControlStateNormal)];
    
    //    [button setBackgroundImage:[UIImage imageNamed:self.imageName] forState:(UIControlStateNormal)];
    
    [button addTarget:self action:@selector(action:) forControlEvents:(UIControlEventTouchUpInside)];
    
    UIButton *cancelButton = [UIButton buttonWithType:(UIButtonTypeSystem)];
    
    if (self.cancelImageName && ![self.cancelImageName isEqualToString:@""])
    {
        cancelButton.frame = CGRectMake(CGRectGetMaxX(button.frame) + 25 , CGRectGetMinY(button.frame), 22, 22);
    }
    else
    {
        cancelButton.frame = CGRectMake(CGRectGetMaxX(button.frame) + 25, CGRectGetMinY(button.frame) + 2, 30, 22);
        
    }
    
    if (self.cancelImageName && ![self.cancelImageName isEqualToString:@""])
    {
        NSString *bundlePath = [[NSBundle mainBundle] pathForResource:@"AlipaySDK" ofType: @"bundle"];
        NSString *imagePath = [bundlePath stringByAppendingPathComponent :self.cancelImageName];
        
        [cancelButton setBackgroundImage:[UIImage imageWithContentsOfFile: imagePath] forState:(UIControlStateNormal)];
    }
    else
    {
        cancelButton.titleLabel.font = [UIFont systemFontOfSize:14];
        
        [cancelButton setTitleColor:[UIColor colorWithRed:198 green:198 blue:198 alpha:1] forState:(UIControlStateNormal)];
        
        [cancelButton setTitle:@"取消" forState:(UIControlStateNormal)];
    }
    
    [cancelButton addTarget:self action:@selector(cancelAction) forControlEvents:(UIControlEventTouchUpInside)];
    
    [view addSubview: cancelButton];
    
    self.titleLabel = [[UILabel alloc]initWithFrame:CGRectMake(CGRectGetMaxX(cancelButton.frame) + 15, CGRectGetMidY(button.frame) - 10 ,[UIScreen mainScreen].bounds.size.width - 198, 20)];
    
    self.titleLabel.font = [UIFont systemFontOfSize:17];
    
    self.titleLabel.textColor = [UIColor whiteColor];
    
    [self.titleLabel setTextAlignment:(NSTextAlignmentCenter)];
    
    [view addSubview: self.titleLabel];
    
    //iPhone
    if ([[NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding] isEqualToString:@"iPhone10,3"] || [[NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding] isEqualToString:@"iPhone10,6"])
    {
       self.webView = [[UIWebView alloc]initWithFrame:CGRectMake(0, CGRectGetMaxY(view.frame), [UIScreen mainScreen].bounds.size.width,( [UIScreen mainScreen].bounds.size.height - 74-44-44))];
        
    }
    else
    {
        self.webView = [[UIWebView alloc]initWithFrame:CGRectMake(0, CGRectGetMaxY(view.frame), [UIScreen mainScreen].bounds.size.width,( [UIScreen mainScreen].bounds.size.height - 44))];
    }
    
    
    
    if (@available(iOS 11.0, *)) {
        self.webView.scrollView.contentInsetAdjustmentBehavior = UIScrollViewContentInsetAdjustmentNever;
    }
    
    self.webView.delegate = self;
    
    self.webView.opaque = NO;
    
    self.webView.allowsInlineMediaPlayback = YES;
    
    self.webView.backgroundColor = [UIColor clearColor];
    
    [self.webView scalesPageToFit];
    
    if (self.pramaString && ![self.pramaString isEqualToString:@""])
    {
        NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"%@?%@",self.urlString,self.pramaString]];
        
        [self.webView loadRequest:[NSURLRequest requestWithURL:url]];
    }
    else
    {
        NSURL *url = [NSURL URLWithString:self.urlString];
        
        [self.webView loadRequest:[NSURLRequest requestWithURL:url]];
    }
    
    
    [self.view addSubview:self.webView];
    
    [view addSubview: button];
    
    UIView *topView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width, statusBar_HEIGHT + 44)];
    
    topView.backgroundColor = [UIColor colorWithRed:25 green:25 blue:25 alpha:1];
    
    [self.view insertSubview:topView atIndex:0];
    
    //    //  桥接 的 代理
    [WebViewJavascriptBridge enableLogging];
    
    self.bridge = [WebViewJavascriptBridge bridgeForWebView:self.webView];
    
    [self.bridge setWebViewDelegate:self];
    
    [self setTitleBridge];
    
    [self payForAli];
    // Do any additional setup after loading the view.
}

- (void)setTitleBridge
{
    [self.bridge registerHandler:@"setTitle" handler:^(id data, WVJBResponseCallback responseCallback) {
        
        self.titleLabel.text = data;
        
    }];
}
//-(void) webViewDidFinishLoad:(UIWebView *)webView
//{
//    [self.titleLabel setText:[self.webView stringByEvaluatingJavaScriptFromString:@"document.title"]];
//}

- (void)action:(UIButton *)sender
{
    if ([self.webView canGoBack])
    {
        [self.webView goBack];
    }
    else
    {
        [self dismissModalViewControllerAnimated:YES];
    }
}

- (void)cancelAction
{
    [self.webView reload];
    
    [self dismissModalViewControllerAnimated:YES];
}

-(void) viewDidDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    self.webView  = nil;
    
    [self.webView removeFromSuperview];
    
}

-(BOOL) webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    NSArray *titleArray = @[@"native",@"h5"];
    //    NSString *requestURL = [request.URL.absoluteString substringWithRange:NSMakeRange(1,7)];
    
    //    NSString *URL = [NSString string]
    
    NSArray *array = [self splitStringWithString:request.URL.absoluteString andIdentifier:@"//"];
    
    NSMutableString *splitURL = [NSMutableString string];
    
    if ([request.URL.absoluteString rangeOfString:@"img"].location != NSNotFound)
    {
        [self imagePicker];
        
        return NO;
    }
    else if ([request.URL.absoluteString rangeOfString:@"tel"].location != NSNotFound)
    {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"tel://%@",[[request.URL.absoluteString componentsSeparatedByString:@":"] lastObject]]]];
        
        return NO;
    }
    else
    {
        if (array.count == 2)
        {
            splitURL = [array lastObject];
        }
        else
        {
            splitURL = [NSMutableString stringWithFormat:@"%@//%@",[array objectAtIndex:1],[array lastObject]];
        }
        if ([titleArray containsObject:[[splitURL componentsSeparatedByString:@"-"] firstObject]])
        {
            
            NSArray* modeArray = [self splitStringForProtocolWithString:splitURL andIdentifier:@"-"];
            
            
            if ([[modeArray firstObject] isEqualToString:[titleArray firstObject]])
            {
                NSMutableDictionary *dic = [[NSUserDefaults standardUserDefaults] valueForKey:@"Protocol"];
                
                NSMutableDictionary *customDic = [[NSUserDefaults standardUserDefaults] valueForKey:@"CustomProtocol"];
                
                if ([dic valueForKey:[modeArray objectAtIndex:1]])
                {
                    SEL aSelector = NSSelectorFromString([dic valueForKey:[modeArray objectAtIndex:1]]);
                    
                    [self performSelector:aSelector withObject:[modeArray lastObject]];
                    
                }
                else if ([customDic valueForKey:[modeArray objectAtIndex:1]])
                {
                    SEL aSelector = NSSelectorFromString([dic valueForKey:[modeArray objectAtIndex:1]]);
                    
                    [self performSelector:aSelector withObject:[modeArray lastObject]];
                }
                else
                {
                    if (self.callBack)
                    {
                        self.callBack(splitURL);
                    }
                }
                [self dismissModalViewControllerAnimated:YES];
                
            }
            else if([[modeArray firstObject] isEqualToString:[titleArray lastObject]])
            {
                NSMutableString *requestURL = [NSMutableString stringWithFormat:@"%@",[modeArray lastObject]];
                
                NSString *header = [[self splitStringWithString:requestURL andIdentifier:@"//"] firstObject];
                
                [requestURL insertString:@":" atIndex:header.length];
                
                NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"%@%@",requestURL,self.pramaString]];
                
                [self.webView loadRequest:[NSURLRequest requestWithURL:url]];
            }
        }
    }
    return YES;
}

- (void)imagePicker
{
    // 创建UIImagePickerController实例
    UIImagePickerController *imagePickerController = [[UIImagePickerController alloc] init];
    // 设置代理
    imagePickerController.delegate = self;
    // 是否允许编辑（默认为NO）
    //    imagePickerController.allowsEditing = YES;
    
    
    // 创建一个警告控制器
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"选取图片" message:nil preferredStyle:UIAlertControllerStyleActionSheet];
    // 设置警告响应事件
    UIAlertAction *cameraAction = [UIAlertAction actionWithTitle:@"拍照" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        
     
            // 设置照片来源为相机
            imagePickerController.sourceType = UIImagePickerControllerSourceTypeCamera;
            
            // 设置进入相机时使用前置或后置摄像头
            imagePickerController.cameraDevice = UIImagePickerControllerCameraDeviceRear;
            
            // 展示选取照片控制器
            [self presentViewController:imagePickerController animated:YES completion:^{}];
    }];
    
    UIAlertAction *photosAction = [UIAlertAction actionWithTitle:@"从相册选择" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                  // 显示界面
            imagePickerController.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
            [self presentViewController:imagePickerController animated:YES completion:^{}];
    }];
    
    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
    }];
    
    // 判断是否支持相机
    if([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
    {
        // 添加警告按钮
        [alert addAction:cameraAction];
    }
    [alert addAction:photosAction];
    [alert addAction:cancelAction];
    // 展示警告控制器
    [self presentViewController:alert animated:YES completion:nil];
    
    
}

#pragma mark - UIImagePickerControllerDelegate
// 完成图片的选取后调用的方法
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info {
    
    // 选取完图片后跳转回原控制器
    [picker dismissViewControllerAnimated:YES completion:nil];
    
    /* 此处参数 info 是一个字典，下面是字典中的键值 （从相机获取的图片和相册获取的图片时，两者的info值不尽相同）
     * UIImagePickerControllerMediaType; // 媒体类型
     * UIImagePickerControllerOriginalImage;  // 原始图片
     * UIImagePickerControllerEditedImage;    // 裁剪后图片
     * UIImagePickerControllerCropRect;       // 图片裁剪区域（CGRect）
     * UIImagePickerControllerMediaURL;       // 媒体的URL
     * UIImagePickerControllerReferenceURL    // 原件的URL
     * UIImagePickerControllerMediaMetadata    // 当数据来源是相机时，此值才有效
     */
    //    NSData* data = UIImageJPEGRepresentation(image, 0.3);
    
    // 从info中将图片取出，并加载到imageView当中
    
    
    
    NSLog(@"%@",[self image2DataURL:[info objectForKey:UIImagePickerControllerOriginalImage]]);
    
    NSData *imageData = UIImageJPEGRepresentation([info objectForKey:UIImagePickerControllerOriginalImage], 1.0);
    
    UIImage *image = [UIImage imageWithData: imageData];
    
        [self.bridge callHandler:@"iosImagePickerAction" data:@{@"imageData":[self image2DataURL:image]} responseCallback:^(id responseData)
     {
         NSLog(@"%@",responseData);
     }] ;
    
}

// 取消选取调用的方法
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker {
    [self dismissViewControllerAnimated:YES completion:nil];
}

// 保存图片后到相册后，回调的相关方法，查看是否保存成功
- (void)image:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo {
    if (error == nil){
        NSLog(@"Image was saved successfully.");
    } else {
        NSLog(@"An error happened while saving the image.");
        NSLog(@"Error = %@", error);
    }
}


-(void)payForAli
{
    [self.bridge registerHandler:@"payForAli" handler:^(id data, WVJBResponseCallback responseCallback) {
        
        [[AlipaySDK defaultService] payOrder:data fromScheme:Scheme callback:^(NSDictionary *resultDic)
         {
             
             [self.bridge callHandler:@"aliPayCallBack" data:resultDic responseCallback:^(id responseData) {
                 
             }];
         }];
    }];
    
}

- (BOOL) imageHasAlpha: (UIImage *) image
{
    CGImageAlphaInfo alpha = CGImageGetAlphaInfo(image.CGImage);
    return (alpha == kCGImageAlphaFirst ||
            alpha == kCGImageAlphaLast ||
            alpha == kCGImageAlphaPremultipliedFirst ||
            alpha == kCGImageAlphaPremultipliedLast);
}
- (NSString *) image2DataURL: (UIImage *) image
{
    NSData *imageData = nil;
    NSString *mimeType = nil;
    
    if ([self imageHasAlpha: image]) {
        imageData = UIImagePNGRepresentation(image);
        mimeType = @"image/png";
    } else {
        imageData = UIImageJPEGRepresentation(image, 0.01);
        mimeType = @"image/jpeg";
    }
    
    return [NSString stringWithFormat:@"data:%@;base64,%@", mimeType,
            [imageData base64EncodedStringWithOptions: 0]];
    
}


- (NSArray *)splitStringWithString:(NSString *)string andIdentifier:(NSString *)identifier
{
    return [string componentsSeparatedByString:identifier];
}

- (NSArray *)splitStringForProtocolWithString:(NSString *)string andIdentifier:(NSString *)identifier
{
    NSMutableArray *array = [self splitStringWithString:string andIdentifier:identifier].mutableCopy;
    
    NSString *tempString = [NSString stringWithFormat:@"%@%@%@%@",[array firstObject],identifier,[array objectAtIndex:1],identifier];
    
    NSMutableArray *result  = [NSMutableArray array];
    
    [result addObject:[array firstObject]];
    
    [result addObject:[array objectAtIndex:1]];
    
    [result addObjectsFromArray:[self splitStringWithString:string andIdentifier:tempString]];
    
    return result;
}

- (NSMutableString *)loadedURL
{
    if (!_loadedURL)
    {
        _loadedURL =[NSMutableString string];
    }
    
    return  _loadedURL;
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */

@end


#endif
