```
Tests start at 2022-06-18T15:54:06Z

========== Testing HMAC-SHA256 ==========
Key:    0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b
Data:   4869205468657265
Hash:   b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7
Expect: b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7
Result: Passed

Key:    4a656665
Data:   7768617420646f2079612077616e7420666f72206e6f7468696e673f
Hash:   5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843
Expect: 5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843
Result: Passed

Key:    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
Data:   dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd...[Truncated]
Hash:   773ea91e36800e46854db8ebd09181a72959098b3ef8c122d9635514ced565fe
Expect: 773ea91e36800e46854db8ebd09181a72959098b3ef8c122d9635514ced565fe
Result: Passed

Key:    0102030405060708090a0b0c0d0e0f10111213141516171819
Data:   cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd...[Truncated]
Hash:   82558a389a443c0ea4cc819899f2083a85f0faa3e578f8077a2e3ff46729665b
Expect: 82558a389a443c0ea4cc819899f2083a85f0faa3e578f8077a2e3ff46729665b
Result: Passed

Key:    0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c
Data:   546573742057697468205472756e636174696f6e
Hash:   a3b6167473100ee06e0c796c2955552b
Expect: a3b6167473100ee06e0c796c2955552b
Result: Passed

Key:    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa...[Truncated]
Data:   54657374205573696e67204c6172676572205468616e20426c6f636b2d53697a...[Truncated]
Hash:   60e431591ee0b67f0d8a26aacbf5b77f8e0bc6213728c5140546040f0ee37f54
Expect: 60e431591ee0b67f0d8a26aacbf5b77f8e0bc6213728c5140546040f0ee37f54
Result: Passed

Key:    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa...[Truncated]
Data:   5468697320697320612074657374207573696e672061206c6172676572207468...[Truncated]
Hash:   9b09ffa71b942fcb27635fbcd5b0e944bfdc63644f0713938a7f51535c3a35e2
Expect: 9b09ffa71b942fcb27635fbcd5b0e944bfdc63644f0713938a7f51535c3a35e2
Result: Passed



========== Testing HMAC-SHA1 ==========
Key:    0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b
Data:   4869205468657265
Hash:   b617318655057264e28bc0b6fb378c8ef146be00
Expect: b617318655057264e28bc0b6fb378c8ef146be00
Result: Passed

Key:    4a656665
Data:   7768617420646f2079612077616e7420666f72206e6f7468696e673f
Hash:   effcdf6ae5eb2fa2d27416d5f184df9c259a7c79
Expect: effcdf6ae5eb2fa2d27416d5f184df9c259a7c79
Result: Passed

Key:    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
Data:   dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd...[Truncated]
Hash:   125d7342b9ac11cd91a39af48aa17b4f63f175d3
Expect: 125d7342b9ac11cd91a39af48aa17b4f63f175d3
Result: Passed

Key:    0102030405060708090a0b0c0d0e0f10111213141516171819
Data:   cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd...[Truncated]
Hash:   4c9007f4026250c6bc8414f9bf50c86c2d7235da
Expect: 4c9007f4026250c6bc8414f9bf50c86c2d7235da
Result: Passed

Key:    0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c
Data:   546573742057697468205472756e636174696f6e
Hash:   4c1a03424b55e07fe7f27be1d58bb9324a9a5a04
Expect: 4c1a03424b55e07fe7f27be1d58bb9324a9a5a04
Result: Passed

Key:    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa...[Truncated]
Data:   54657374205573696e67204c6172676572205468616e20426c6f636b2d53697a...[Truncated]
Hash:   aa4ae5e15272d00e95705637ce8a3b55ed402112
Expect: aa4ae5e15272d00e95705637ce8a3b55ed402112
Result: Passed

Key:    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa...[Truncated]
Data:   54657374205573696E67204C6172676572205468616E20426C6F636B2D53697A...[Truncated]
Hash:   e8e99d0f45237d786d6bbaa7965c7808bbff1a91
Expect: e8e99d0f45237d786d6bbaa7965c7808bbff1a91
Result: Passed



========== ALL tests passed ==========
```
