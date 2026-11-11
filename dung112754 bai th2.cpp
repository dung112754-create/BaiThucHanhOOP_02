#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

class PhuongTien {
private:
    int maSoXe;
    string phanLoaiXe;
    int thoiDiemVao;
    int thoiLuongDau;

public:
    PhuongTien();
    PhuongTien(int id, string loai, int vao, int tg);

    int layMaXe() const;
    string layLoaiXe() const;
    double tinhTienPhi() const;
    void xuatThongTin() const;
};

PhuongTien::PhuongTien() : maSoXe(0), phanLoaiXe(""), thoiDiemVao(0), thoiLuongDau(0) {}

PhuongTien::PhuongTien(int id, string loai, int vao, int tg)
    : maSoXe(id), phanLoaiXe(loai), thoiDiemVao(vao), thoiLuongDau(tg) {}

int PhuongTien::layMaXe() const { return maSoXe; }
string PhuongTien::layLoaiXe() const { return phanLoaiXe; }

double PhuongTien::tinhTienPhi() const {
    double donGia = (phanLoaiXe == "Car") ? 6.0 : (phanLoaiXe == "Motorcycle") ? 4.0 : 2.0;
    bool gioCaoDiem = (thoiDiemVao >= 6 && thoiDiemVao <= 9) || (thoiDiemVao >= 15 && thoiDiemVao <= 18);
    double tongTien = 0;
    for (int buoc = 0; buoc < thoiLuongDau; ++buoc) {
        if (buoc < 2) tongTien += donGia * (gioCaoDiem ? 1.30 : 0.95);
        else          tongTien += donGia * 0.90;
    }
    return tongTien;
}

void PhuongTien::xuatThongTin() const {
    cout << "  Ma Xe: " << maSoXe << " | Loai: " << phanLoaiXe
         << " | Gio Vao: " << thoiDiemVao << "h | Thoi Luong: " << thoiLuongDau << "h"
         << " | Cuoc Phi: $" << fixed << setprecision(2) << tinhTienPhi() << "\n";
}

class BaiDoXe {
private:
    int maSoBai;
    char kyHieuKichThuoc;
    static const int SUC_CHUA_TOI_DA = 3;
    PhuongTien danhSachXe[SUC_CHUA_TOI_DA];
    int soLuongXeHienTai;

public:
    BaiDoXe();
    BaiDoXe(int ma, char kt);

    int layMaBai() const;
    char layKyHieuKichThuoc() const;
    int laySoLuongXeHienTai() const;
    int laySucChuaVatLy() const;

    bool kiemTraDayBai() const;
    bool themXeMoi(const PhuongTien& pt);
    bool xoaXeKhoiBai(int idXe);
    int timViTriXe(int idXe) const;
    void xuatThongTinBai() const;
    bool kiemTraLoaiXe(string loai, int& dem, double& tongPhi) const;
    bool chuaXeNay(int idXe) const;
};

BaiDoXe::BaiDoXe() : maSoBai(0), kyHieuKichThuoc('S'), soLuongXeHienTai(0) {}

BaiDoXe::BaiDoXe(int ma, char kt) : maSoBai(ma), kyHieuKichThuoc(kt), soLuongXeHienTai(0) {}

int BaiDoXe::layMaBai() const { return maSoBai; }
char BaiDoXe::layKyHieuKichThuoc() const { return kyHieuKichThuoc; }
int BaiDoXe::laySoLuongXeHienTai() const { return soLuongXeHienTai; }
int BaiDoXe::laySucChuaVatLy() const {
    return (kyHieuKichThuoc == 'S') ? 1 : (kyHieuKichThuoc == 'M') ? 2 : 3;
}

bool BaiDoXe::kiemTraDayBai() const {
    return soLuongXeHienTai >= laySucChuaVatLy();
}

bool BaiDoXe::themXeMoi(const PhuongTien& pt) {
    if (kiemTraDayBai()) return false;
    danhSachXe[soLuongXeHienTai] = pt;
    soLuongXeHienTai++;
    return true;
}

int BaiDoXe::timViTriXe(int idXe) const {
    for (int i = 0; i < soLuongXeHienTai; ++i) {
        if (danhSachXe[i].layMaXe() == idXe) {
            return i;
        }
    }
    return -1;
}

bool BaiDoXe::chuaXeNay(int idXe) const {
    return timViTriXe(idXe) != -1;
}

bool BaiDoXe::xoaXeKhoiBai(int idXe) {
    int viTri = timViTriXe(idXe);
    if (viTri == -1) return false;
    for (int i = viTri; i < soLuongXeHienTai - 1; ++i) {
        danhSachXe[i] = danhSachXe[i + 1];
    }
    soLuongXeHienTai--;
    return true;
}

void BaiDoXe::xuatThongTinBai() const {
    int sucChua = laySucChuaVatLy();
    cout << "Khu vuc bai " << maSoBai << " [" << kyHieuKichThuoc << "] "
         << soLuongXeHienTai << "/" << sucChua << " cho da dung\n";
    for (int i = 0; i < soLuongXeHienTai; ++i) {
        danhSachXe[i].xuatThongTin();
    }
}

bool BaiDoXe::kiemTraLoaiXe(string loai, int& dem, double& tongPhi) const {
    dem = 0;
    tongPhi = 0;
    for (int i = 0; i < soLuongXeHienTai; ++i) {
        if (danhSachXe[i].layLoaiXe() == loai) {
            ++dem;
            tongPhi += danhSachXe[i].tinhTienPhi();
        }
    }
    return dem > 0;
}

class GiaoDienHeThong {
private:
    static const int SO_LUONG_BAI_MAX = 10;
    BaiDoXe danhSachBaiDo[SO_LUONG_BAI_MAX];
    int soLuongBaiHienTai;

    int sinhMaSoNgauNhien() const;
    bool kiemTraTrungMaBai(int maBai) const;
    bool kiemTraTrungMaXeToanHeThong(int maXe) const;

public:
    GiaoDienHeThong();
    void khoiChuyenMenu();
};

GiaoDienHeThong::GiaoDienHeThong() : soLuongBaiHienTai(0) {
    srand(static_cast<unsigned int>(time(0)));
}

int GiaoDienHeThong::sinhMaSoNgauNhien() const {
    return 1000 + rand() % 9000;
}

bool GiaoDienHeThong::kiemTraTrungMaBai(int maBai) const {
    for (int i = 0; i < soLuongBaiHienTai; ++i) {
        if (danhSachBaiDo[i].layMaBai() == maBai) return true;
    }
    return false;
}

bool GiaoDienHeThong::kiemTraTrungMaXeToanHeThong(int maXe) const {
    for (int i = 0; i < soLuongBaiHienTai; ++i) {
        if (danhSachBaiDo[i].chuaXeNay(maXe)) return true;
    }
    return false;
}

void GiaoDienHeThong::khoiChuyenMenu() {
    int luaChon;
    while (true) {
        cout << "\n===== QUAN LY HE THONG BAI DO XE (TIENG VIET) =====\n";
        cout << "1. Tao bai do xe moi\n";
        cout << "2. Dua xe vao bai do\n";
        cout << "3. Xoa bo bai do xe\n";
        cout << "4. Lay xe ra khoi he thong\n";
        cout << "5. Hien thi tat ca cac bai do\n";
        cout << "6. Hien thi xe theo ma bai\n";
        cout << "7. Thong ke va liet ke xe theo loai\n";
        cout << "8. Bao cao cuoc phi do xe trung binh\n";
        cout << "9. Thoat chuong trinh\n";
        cout << "Moi ban chon chuc nang: ";
        
        if (!(cin >> luaChon)) {
            cout << "Loi nhap lieu! Vui long nhap so nguyen hop le.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (luaChon) {
            case 1: {
                if (soLuongBaiHienTai >= SO_LUONG_BAI_MAX) {
                    cout << "Da dat gioi han toi da so luong bai do (" << SO_LUONG_BAI_MAX << " bai)!\n";
                    break;
                }
                int maBai; char kichThuoc;
                cout << "Nhap ma bai do: ";
                while (!(cin >> maBai)) {
                    cout << "Nhap sai! Nhap lai ma bai: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                if (kiemTraTrungMaBai(maBai)) {
                    int maMoi = sinhMaSoNgauNhien();
                    while (kiemTraTrungMaBai(maMoi)) {
                        maMoi = sinhMaSoNgauNhien();
                    }
                    cout << "Canh bao: Ma bai bi trung! He thong da tu dong gan ma moi: " << maMoi << "\n";
                    maBai = maMoi;
                }
                cout << "Nhap kich thuoc bai (S/M/L): ";
                cin >> kichThuoc;
                while (kichThuoc != 'S' && kichThuoc != 'M' && kichThuoc != 'L' && kichThuoc != 's' && kichThuoc != 'm' && kichThuoc != 'l') {
                    cout << "Kich thuoc khong hop le! Vui long nhap lai (S/M/L): ";
                    cin >> kichThuoc;
                }
                if (kichThuoc >= 's' && kichThuoc <= 'z') kichThuoc = kichThuoc - 32;

                danhSachBaiDo[soLuongBaiHienTai] = BaiDoXe(maBai, kichThuoc);
                soLuongBaiHienTai++;
                cout << "Tao thanh cong bai do " << maBai << " v?i kich thuoc [" << kichThuoc << "]\n";
                break;
            }
            case 2: {
                if (soLuongBaiHienTai == 0) {
                    cout << "He thong chua co bai do nao! Vui long tao bai do truoc.\n";
                    break;
                }
                int maXe, gioVao, thoiLuong; string loaiXe;
                cout << "Nhap ma so xe: ";
                while (!(cin >> maXe)) {
                    cout << "Nhap sai! Nhap lai ma xe: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                if (kiemTraTrungMaXeToanHeThong(maXe)) {
                    int maMoi = sinhMaSoNgauNhien();
                    while (kiemTraTrungMaXeToanHeThong(maMoi)) {
                        maMoi = sinhMaSoNgauNhien();
                    }
                    cout << "Canh bao: Ma xe da ton tai! He thong da tu dong doi thanh ma moi: " << maMoi << "\n";
                    maXe = maMoi;
                }
                cout << "Nhap loai xe (Car/Motorcycle/Bicycle): ";
                cin >> loaiXe;
                cout << "Nhap gio xe vao (0-23): ";
                while (!(cin >> gioVao) || gioVao < 0 || gioVao > 23) {
                    cout << "Gio khong hop le (tu 0 den 23)! Nhap lai: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cout << "Nhap thoi luong do (>= 1): ";
                while (!(cin >> thoiLuong) || thoiLuong <= 0) {
                    cout << "Thoi luong khong hop le! Nhap lai: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                PhuongTien xeMoi(maXe, loaiXe, gioVao, thoiLuong);
                bool daXepXe = false;
                for (int i = 0; i < soLuongBaiHienTai; ++i) {
                    if (!danhSachBaiDo[i].kiemTraDayBai()) {
                        danhSachBaiDo[i].themXeMoi(xeMoi);
                        daXepXe = true;
                        cout << "Da xep xe " << maXe << " vao bai " << danhSachBaiDo[i].layMaBai()
                             << " | Phi du kien: $" << fixed << setprecision(2) << xeMoi.tinhTienPhi() << "\n";
                        break;
                    }
                }
                if (!daXepXe) {
                    cout << "Tat ca cac bai do trong he thong deu da day cho!\n";
                }
                break;
            }
            case 3: {
                int maBaiXoa;
                cout << "Nhap ma bai do can xoa: ";
                while (!(cin >> maBaiXoa)) {
                    cout << "Nhap sai! Nhap lai ma bai: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                int viTriBai = -1;
                for (int i = 0; i < soLuongBaiHienTai; ++i) {
                    if (danhSachBaiDo[i].layMaBai() == maBaiXoa) {
                        viTriBai = i;
                        break;
                    }
                }
                if (viTriBai == -1) {
                    cout << "Khong tim thay bai do co ma " << maBaiXoa << "\n";
                } else {
                    for (int i = viTriBai; i < soLuongBaiHienTai - 1; ++i) {
                        danhSachBaiDo[i] = danhSachBaiDo[i + 1];
                    }
                    soLuongBaiHienTai--;
                    cout << "Da xoa bo bai do " << maBaiXoa << " thanh cong!\n";
                }
                break;
            }
            case 4: {
                int maXeLay;
                cout << "Nhap ma xe can lay ra: ";
                while (!(cin >> maXeLay)) {
                    cout << "Nhap sai! Nhap lai ma xe: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                bool daTimThay = false;
                for (int i = 0; i < soLuongBaiHienTai; ++i) {
                    if (danhSachBaiDo[i].chuaXeNay(maXeLay)) {
                        danhSachBaiDo[i].xoaXeKhoiBai(maXeLay);
                        daTimThay = true;
                        cout << "Da lay xe " << maXeLay << " ra khoi bai " << danhSachBaiDo[i].layMaBai() << "\n";
                        break;
                    }
                }
                if (!daTimThay) {
                    cout << "Khong tim thay xe co ma " << maXeLay << " trong he thong!\n";
                }
                break;
            }
            case 5: {
                if (soLuongBaiHienTai == 0) {
                    cout << "He thong hien chua co bai do nao.\n";
                } else {
                    cout << "\n--- DANH SACH TOAN BO CAC BAI DO TRONG HE THONG ---\n";
                    for (int i = 0; i < soLuongBaiHienTai; ++i) {
                        danhSachBaiDo[i].xuatThongTinBai();
                        cout << "--------------------------------------------\n";
                    }
                }
                break;
            }
            case 6: {
                int maBaiXem;
                cout << "Nhap ma bai can xem thong tin: ";
                while (!(cin >> maBaiXem)) {
                    cout << "Nhap sai! Nhap lai ma bai: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                bool thayBai = false;
                for (int i = 0; i < soLuongBaiHienTai; ++i) {
                    if (danhSachBaiDo[i].layMaBai() == maBaiXem) {
                        danhSachBaiDo[i].xuatThongTinBai();
                        thayBai = true;
                        break;
                    }
                }
                if (!thayBai) {
                    cout << "Khong tim thay bai do co ma " << maBaiXem << "\n";
                }
                break;
            }
            case 7: {
                string loaiCanTim;
                cout << "Nhap loai xe can thong ke (Car/Motorcycle/Bicycle): ";
                cin >> loaiCanTim;
                cout << "\n--- KET QUA LIET KE XE THEO LOAI [" << loaiCanTim << "] ---\n";
                bool coXeThoaMan = false;
                for (int i = 0; i < soLuongBaiHienTai; ++i) {
                    int demXe = 0; double tongTienBai = 0;
                    if (danhSachBaiDo[i].kiemTraLoaiXe(loaiCanTim, demXe, tongTienBai)) {
                        cout << "Bai do " << danhSachBaiDo[i].layMaBai() << " co " << demXe << " xe.\n";
                        coXeThoaMan = true;
                    }
                }
                if (!coXeThoaMan) {
                    cout << "Khong co xe nao thuoc loai nay trong toan bo he thong.\n";
                }
                break;
            }
            case 8: {
                string mangLoaiXe[3] = {"Car", "Motorcycle", "Bicycle"};
                cout << "\n--- BAO CAO PHI DO XE TRUNG BINH THEO LOAI ---\n";
                for (int t = 0; t < 3; ++t) {
                    int tongSoLuongXe = 0;
                    double tongGiaTriPhi = 0;
                    for (int i = 0; i < soLuongBaiHienTai; ++i) {
                        int dem = 0; double tien = 0;
                        if (danhSachBaiDo[i].kiemTraLoaiXe(mangLoaiXe[t], dem, tien)) {
                            tongSoLuongXe += dem;
                            tongGiaTriPhi += tien;
                        }
                    }
                    cout << "Loai [" << mangLoaiXe[t] << "]: ";
                    if (tongSoLuongXe == 0) {
                        cout << "Khong co xe nao\n";
                    } else {
                        cout << tongSoLuongXe << " xe | Tong phi: $" << fixed << setprecision(2) << tongGiaTriPhi
                             << " | Trung binh: $" << tongGiaTriPhi / tongSoLuongXe << "\n";
                    }
                }
                break;
            }
            case 9:
                cout << "Dang thoat chuong trinh. Tam biet!\n";
                return;
            default:
                cout << "Lua chon khong hop le! Vui long chi chon tu 1 den 9.\n";
                break;
        }
    }
}

int main() {
    GiaoDienHeThong heThongQuanLy;
    heThongQuanLy.khoiChuyenMenu();
    return 0;
}
