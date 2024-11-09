#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class M8A1 : public Item {
public:
    M8A1() : Item(m8a1, ItemType::CUSTOM) {
        maxAmmo = 30;
        currentAmmo = maxAmmo;
        fireRate = 0.2f;
        lastFireTime = 0.0f;
        isAutomatic = false;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre M8A1 est vide. Rechargez.");
            return;
        }

        if (isAutomatic) {
            autoShoot(player);
        } else {
            singleShoot(player);
        }
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre M8A1 est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::BULLET)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::BULLET, maxAmmo);
            player.sendMessage("Votre M8A1 a été rechargé avec succès.");
        } else {
            player.sendMessage("Vous n'avez pas assez de munitions pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;
    float fireRate;
    float lastFireTime;
    bool isAutomatic;

    void singleShoot(Player &player) {
        float currentTime = Minecraft::getTime();
        if (currentTime - lastFireTime < fireRate) {
            player.sendMessage("Veuillez attendre avant de tirer à nouveau.");
            return;
        }
        shoot(player);
    }

    void autoShoot(Player &player) {
        float currentTime = Minecraft::getTime();
        if (currentTime - lastFireTime < fireRate) {
            player.sendMessage("Veuillez attendre avant de tirer à nouveau.");
            return;
        }
        shoot(player);
        shoot(player);
    }

    void shoot(Player &player) {
        Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);
        bullet.setDamage(20);
        bullet.setVelocity(player.getDirection() * 3.0f);
        player.playSound(SoundType::SHOOT);
        player.sendMessage("Bang. La M8A1 a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<M8A1>(m8a1);
}