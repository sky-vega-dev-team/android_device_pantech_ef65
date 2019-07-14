#
# Copyright (C) 2015-2016 The CyanogenMod Project
#               2017-2018 The LineageOS Projec
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Inherit from ef65 device
$(call inherit-product, device/pantech/ef65/ef65.mk)

# Inherit some common LineageOS stuff.
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)

# Call the proprietary setup
$(call inherit-product-if-exists, vendor/pantech/ef65/ef65-vendor.mk)

PRODUCT_NAME := lineage_ef65
PRODUCT_DEVICE := ef65
PRODUCT_MANUFACTURER := PANTECH
PRODUCT_MODEL := IM-A920

PRODUCT_BRAND := PANTECH
TARGET_VENDOR := PANTECH
TARGET_VENDOR_PRODUCT_NAME := ef65
TARGET_VENDOR_DEVICE_NAME := ef65

PRODUCT_BUILD_PROP_OVERRIDES += \
    PRIVATE_BUILD_DESC="msm8974-user 4.4.2 KVT49L EF65S_KK.006 release-keys" 

BUILD_FINGERPRINT := VEGA/VEGA_IM-A920S/ef65s:4.4.2/KVT49L/EF65S_KK.006:user/release-keys
