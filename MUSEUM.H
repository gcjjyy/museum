#ifndef __MUSEUM_H__
#define __MUSEUM_H__

class Museum {
   public:
    static Museum *shared();

   public:
    void run();

   private:
    Museum();
    ~Museum();

   private:
    static Museum *m_shared;
};

#endif
