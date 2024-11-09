#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class CHICOM_CQB : public Item {
public:
    CHICOM_CQB() : Item(chicom_cqb, ItemType::CUSTOM) {
        maxAmmo = 24;
        currentAmmo = maxAmmo;
        fireRate = 0.4f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre CHICOM CQB est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre CHICOM CQB est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::BULLET)) {
            int ammoToReload = maxAmmo - currentAmmo;
            currentAmmo += ammoToReload;
            player.removeItem(ItemType::BULLET, ammoToReload);
            player.sendMessage("Votre CHICOM CQB a été rechargé avec succès.");
        } else {
            player.sendMessage("Vous n'avez pas assez de munitions pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;
    float fireRate;
    float lastFireTime;

    void shoot(Player &player) {
        Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);  
        bullet.setDamage(9);
        bullet.setVelocity(player.getDirection() * 4.5f);
        player.playSound(SoundType::GUNSHOT);
        player.sendMessage("Tir. Le CHICOM CQB a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<CHICOM_CQB>(chicom_cqb);
}